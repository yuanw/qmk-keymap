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
import socket
import subprocess
import sys
import time
import urllib.parse
import urllib.request
from pathlib import Path

ROOT = Path(__file__).parents[1]
PORT_START = 8765
IDLE_TIMEOUT = 1800  # seconds

last_request = [time.time()]


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(ROOT), **kwargs)

    def do_GET(self):
        last_request[0] = time.time()
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


def main():
    if len(sys.argv) > 2 and sys.argv[1] == "--serve":
        port = int(sys.argv[2])
        srv = http.server.ThreadingHTTPServer(("127.0.0.1", port), Handler)
        while time.time() - last_request[0] < IDLE_TIMEOUT:
            srv.handle_request()
        srv.server_close()
        return

    if len(sys.argv) < 2:
        raise SystemExit("usage: serve_trainer.py <page.html relative to the repo>")
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
    subprocess.run(["open", url], check=False)


if __name__ == "__main__":
    main()
