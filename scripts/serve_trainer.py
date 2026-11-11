#!/usr/bin/env python3
"""Open a generated page over http://127.0.0.1 instead of file://.

LibreWolf's content sandbox will not read files outside the usual user
directories, so file:// URLs from the repo just fail to load. This starts a
detached, loopback-only server for the repo (reused if one is already up) and
hands the browser a localhost URL. The server quits after an idle while.

    serve_trainer.py keymap-drawer/dilemma_trainer.html
    serve_trainer.py --serve 8765        # internal: the server itself
"""

import http.server
import os
import socket
import subprocess
import sys
import time
import urllib.parse
import urllib.request
from pathlib import Path

# Repo root by default; the nix package points this at its own pages dir.
ROOT = Path(os.environ.get("TRAINER_ROOT", Path(__file__).parents[1]))
PORT_START = 8765
IDLE_TIMEOUT = 1800  # seconds

last_request = [time.time()]
quit_requested = [False]


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(ROOT), **kwargs)

    def do_GET(self):
        last_request[0] = time.time()
        # The server is detached, so you cannot Ctrl-C it: this is how `just stop`
        # shuts it down. Loopback-only, and the worst it does is stop the server.
        if self.path.rstrip("/") == "/__quit__":
            self.send_response(204)
            self.end_headers()
            quit_requested[0] = True   # server.shutdown() only unwinds serve_forever, not this loop
            return
        super().do_GET()

    def log_message(self, *args):
        pass


def serving(port, page):
    """Is our server (or an equivalent one) already answering this page?"""
    try:
        with urllib.request.urlopen(
            f"http://127.0.0.1:{port}/{urllib.parse.quote(page)}", timeout=0.5
        ) as r:
            return r.status == 200
    except OSError:
        return False


def free(port):
    with socket.socket() as s:
        return s.connect_ex(("127.0.0.1", port)) != 0


def stop():
    """Ask every server we might have started (our port range) to quit."""
    stopped = []
    for port in range(PORT_START, PORT_START + 20):
        try:
            with urllib.request.urlopen(f"http://127.0.0.1:{port}/__quit__", timeout=0.5) as r:
                if r.status == 204:
                    stopped.append(port)
        except OSError:
            pass
    print("stopped " + ", ".join(map(str, stopped)) if stopped else "nothing running")


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--stop":
        stop()
        return

    if len(sys.argv) > 2 and sys.argv[1] == "--serve":
        port = int(sys.argv[2])
        srv = http.server.ThreadingHTTPServer(("127.0.0.1", port), Handler)
        while not quit_requested[0] and time.time() - last_request[0] < IDLE_TIMEOUT:
            srv.handle_request()
        srv.server_close()
        return

    if len(sys.argv) < 2:
        raise SystemExit("usage: serve_trainer.py [--stop | <page.html relative to the served root>]")
    page = sys.argv[1]

    candidates = range(PORT_START, PORT_START + 20)
    port = next((p for p in candidates if serving(p, page)), None)
    if port is None:
        port = next(p for p in candidates if free(p))
        subprocess.Popen(
            [sys.executable, __file__, "--serve", str(port)],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            start_new_session=True,
        )
        for _ in range(50):
            if serving(port, page):
                break
            time.sleep(0.1)

    url = f"http://127.0.0.1:{port}/{urllib.parse.quote(page)}"
    print(url)
    opener = "open" if sys.platform == "darwin" else "xdg-open"
    subprocess.run([opener, url], check=False)


if __name__ == "__main__":
    main()
