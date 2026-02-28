#!/usr/bin/env python3
"""Expand LAYOUT_LR (36 keys) to LAYOUT_let_no_bottom_row (48 keys) for imprint."""

import json
import sys

def expand_layer(layer_36):
    """
    Expand a 36-key LAYOUT_LR layer to 48-key LAYOUT_let_no_bottom_row.

    LAYOUT_LR input (36 keys):
      L00 L01 L02 L03 L04    R00 R01 R02 R03 R04   <- indices 0-9
      L10 L11 L12 L13 L14    R10 R11 R12 R13 R14   <- indices 10-19
      L20 L21 L22 L23 L24    R20 R21 R22 R23 R24   <- indices 20-29
                LT0 LT1 LT2       RT0 RT1 RT2      <- indices 30-35

    LAYOUT_let_no_bottom_row output (48 keys):
      X   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04 X    <- Row 0 (12 keys)
      B   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14 W    <- Row 1 (12 keys)
      X   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24 X    <- Row 2 (12 keys)
                    X   X   X         X   X   X           <- Top thumbs (6 keys)
                  LT0 LT1 LT2      RT2 RT0 RT1            <- Bottom thumbs (6 keys)
    """
    layer_48 = []

    # Row 0: X + L00-L04 + R00-R04 + X
    layer_48.append("KC_NO")
    layer_48.extend(layer_36[0:5])   # L00-L04
    layer_48.extend(layer_36[5:10])  # R00-R04
    layer_48.append("KC_NO")

    # Row 1: B + L10-L14 + R10-R14 + W
    layer_48.append("KC_B")
    layer_48.extend(layer_36[10:15])  # L10-L14
    layer_48.extend(layer_36[15:20])  # R10-R14
    layer_48.append("KC_W")

    # Row 2: X + L20-L24 + R20-R24 + X
    layer_48.append("KC_NO")
    layer_48.extend(layer_36[20:25])  # L20-L24
    layer_48.extend(layer_36[25:30])  # R20-R24
    layer_48.append("KC_NO")

    # Top thumbs: 6 X's
    layer_48.extend(["KC_NO"] * 6)

    # Bottom thumbs: LT0 LT1 LT2 + RT2 + RT0 RT1
    layer_48.extend(layer_36[30:33])  # LT0-LT2
    layer_48.append(layer_36[35])     # RT2 (inner-most right thumb)
    layer_48.extend(layer_36[33:35])  # RT0-RT1

    return layer_48

def main():
    filename = sys.argv[1] if len(sys.argv) > 1 else "imprint.json"

    with open(filename, "r") as f:
        data = json.load(f)

    # Update layout name
    data["layout"] = "LAYOUT_let_no_bottom_row"

    # Expand each layer
    expanded_layers = []
    for layer in data["layers"]:
        if len(layer) == 36:
            expanded_layers.append(expand_layer(layer))
        else:
            # Already expanded or different size
            expanded_layers.append(layer)

    data["layers"] = expanded_layers

    with open(filename, "w") as f:
        json.dump(data, f)

    print(f"Expanded {len(expanded_layers)} layers from 36 to 48 keys")

if __name__ == "__main__":
    main()
