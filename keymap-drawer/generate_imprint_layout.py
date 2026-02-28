#!/usr/bin/env python3
"""
Generate a custom keymap-drawer physical layout for the Cyboard Imprint.

The official info.json places the top-arc thumb keys (indices 36-41) at
y=3.5-4, only 0.5-1 unit below the bottom finger row (y=3), causing visual
overlap in the SVG. This script generates imprint_layout.json with the same
finger key positions but the thumb keys shifted down to a proper arc.

Active thumb arc positions (indices 36-41 in LAYOUT_let_no_bottom_row):
  Left:  LT0 (outer) → LT1 (mid) → LT2 (inner)  x=4→5→6, y=4.75→4.75→4.5
  Right: RT0 (inner) → RT1 (mid) → RT2 (outer)  x=10→11→12, y=4.5→4.75→4.75

Hidden bottom cluster (indices 42-47) keeps original positions (irrelevant,
hidden by hide_empty_keys.py).
"""

import json
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
INFO_JSON  = Path(__file__).parent.parent / "imprint/keyboards/cyboard/imprint/imprint_letters_only_no_bottom_row/info.json"
OUTPUT     = SCRIPT_DIR / "imprint_layout.json"

# New positions for the 6 active thumb keys (indices 36-41).
# Arc dips slightly toward outer keys to match the physical keyboard.
THUMB_OVERRIDES = {
    36: {"x": 4,  "y": 4.75},  # LT0 = ESC_WIN  (outer left)
    37: {"x": 5,  "y": 4.75},  # LT1 = SPC_NAV  (middle left)
    38: {"x": 6,  "y": 4.5},   # LT2 = ARCANE   (inner left,  slightly higher)
    39: {"x": 10, "y": 4.5},   # RT0 = REP_TXT  (inner right, slightly higher)
    40: {"x": 11, "y": 4.75},  # RT1 = R_NUM    (middle right)
    41: {"x": 12, "y": 4.75},  # RT2 = BSPC_FUN (outer right)
}


def main():
    data = json.loads(INFO_JSON.read_text())
    layout = data["layouts"]["LAYOUT_let_no_bottom_row"]["layout"]

    for idx, override in THUMB_OVERRIDES.items():
        layout[idx].update(override)

    # Write only the layouts section — keymap-drawer only needs this part.
    output = {"layouts": data["layouts"]}
    OUTPUT.write_text(json.dumps(output, indent=2))
    print(f"Generated {OUTPUT}")
    for idx, pos in THUMB_OVERRIDES.items():
        print(f"  [{idx}] x={pos['x']} y={pos['y']}")


if __name__ == "__main__":
    main()
