#!/usr/bin/env python3
"""
Add a drawing-only virtual trackball to the Imprint keymap-drawer files.

This does not modify QMK firmware layout data. It appends one extra physical
layout entry and one matching key spec to every rendered layer.
"""

import json
from pathlib import Path

import yaml

SCRIPT_DIR = Path(__file__).parent
LAYOUT_JSON = SCRIPT_DIR / "imprint_layout.json"
KEYMAP_YAML = SCRIPT_DIR / "imprint_output.yaml"

# Position the virtual balls in the center gap between the two halves.
TRACKBALL_LAYOUTS = [
    {
        "x": 6.8,
        "y": 2.35,
        "w": 1.8,
        "h": 1.8,
        "type": "trackball",
    },
    {
        "x": 8.8,
        "y": 2.35,
        "w": 1.8,
        "h": 1.8,
        "type": "trackball",
    },
]

TRACKBALL_KEYS = [
    {
        "t": "Trackball",
        "h": "Left",
        "type": "trackball",
    },
    {
        "t": "Trackball",
        "h": "Right",
        "type": "trackball",
    },
]

TRACKBALL_STYLE = """\
rect.trackball {
    fill: #b32020;
    rx: 999;
    ry: 999;
}
text.trackball {
    fill: white;
    stroke: #24292e;
    stroke-width: 3px;
    paint-order: stroke;
}
"""


def main() -> None:
    layout_data = json.loads(LAYOUT_JSON.read_text(encoding="utf-8"))
    layout = layout_data["layouts"]["LAYOUT_let_no_bottom_row"]["layout"]

    # Idempotent: remove previous virtual entries if this script is run twice.
    layout[:] = [entry for entry in layout if entry.get("type") != "trackball"]
    layout.extend(TRACKBALL_LAYOUTS)
    LAYOUT_JSON.write_text(json.dumps(layout_data, indent=2) + "\n", encoding="utf-8")

    keymap_data = yaml.safe_load(KEYMAP_YAML.read_text(encoding="utf-8"))
    for rows in keymap_data["layers"].values():
        for row in rows:
            row[:] = [key for key in row if not (isinstance(key, dict) and key.get("type") == "trackball")]
        rows[-1].extend(key.copy() for key in TRACKBALL_KEYS)

    draw_config = keymap_data.setdefault("draw_config", {})
    draw_config["svg_extra_style"] = TRACKBALL_STYLE

    KEYMAP_YAML.write_text(yaml.safe_dump(keymap_data, sort_keys=False, allow_unicode=True), encoding="utf-8")
    print(f"Added virtual trackball to {KEYMAP_YAML} and {LAYOUT_JSON}")


if __name__ == "__main__":
    main()
