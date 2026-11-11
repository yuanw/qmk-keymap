#!/usr/bin/env python3
"""Copy one layout out of a keyboard json into keymap-drawer/<target>_layout.json.

The trainer pages need key geometry, and flake.nix builds them without the
firmware submodules (git submodules are not part of a flake source), so the
geometry is committed next to the keymap YAML. Refresh after a board change:

    extract_layout.py charybdis/keyboards/bastardkb/charybdis/3x5/info.json LAYOUT out.json
"""

import json
import sys
from pathlib import Path


def main():
    src, layout, out = sys.argv[1:4]
    layouts = json.load(open(src))["layouts"]
    if layout not in layouts:
        raise SystemExit(f"{src}: no layout {layout!r}, has {list(layouts)}")
    Path(out).write_text(json.dumps({"layouts": {layout: layouts[layout]}}, indent=1) + "\n")
    print(f"{out}: {layout} — {len(layouts[layout]['layout'])} keys")


if __name__ == "__main__":
    main()
