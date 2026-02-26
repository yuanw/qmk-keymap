#!/usr/bin/env python3
"""
Generate mouseless grid configuration from QMK keymap JSON.

This script extracts the BASE layer from a QMK keymap JSON file and generates
mouseless grid key strings for the grid overlay configuration.

Usage:
    python scripts/generate_mouseless_grid.py keymap-drawer/charybdis.json
    just mouseless [charybdis]
"""

import json
import sys
from datetime import datetime
from pathlib import Path


def qmk_to_mouseless_key(keycode: str) -> str:
    """
    Convert QMK keycode to mouseless physical key character.

    Args:
        keycode: QMK keycode (e.g., "KC_Q", "HRM_C", "LR_DOT")

    Returns:
        Single character for mouseless grid (e.g., "Q", "C", ".")
    """
    # Strip common prefixes
    if keycode.startswith('HRM_'):
        return keycode[4:]  # HRM_C → C
    if keycode.startswith('LR_'):
        keycode = keycode[3:]  # LR_DOT → DOT
    if keycode.startswith('KC_'):
        keycode = keycode[3:]  # KC_Q → Q

    # Map special QMK keycodes to physical characters
    key_map = {
        'COMM': ',',
        'QUOT': "'",
        'SCLN': ';',
        'SLASH': '/',
        'MINS': '-',
        'EQL': '=',
        'DOT': '.',
    }

    return key_map.get(keycode, keycode)


def create_grid_string(layer_keys: list[str]) -> str:
    """
    Create mouseless grid string from BASE layer keys.

    The Charybdis 3x5 layout has 35 keys total stored in QMK as:
    - Keys 0-14: Left hand (row-major: 3 rows × 5 cols)
    - Keys 15-29: Right hand (row-major: 3 rows × 5 cols)
    - Keys 30-34: Thumb cluster (not used in mouseless grid)

    QMK stores keys as:
      L Row 0: [0, 1, 2, 3, 4]     R Row 0: [15, 16, 17, 18, 19]
      L Row 1: [5, 6, 7, 8, 9]     R Row 1: [20, 21, 22, 23, 24]
      L Row 2: [10, 11, 12, 13, 14] R Row 2: [25, 26, 27, 28, 29]

    For mouseless 10×3 grid, we want left and right hands side-by-side,
    reading each row from left pinky to right pinky.

    Args:
        layer_keys: List of all keycodes from BASE layer

    Returns:
        Grid string with 6 groups of 5 chars: "GROUP1 GROUP2 GROUP3 GROUP4 GROUP5 GROUP6"
        Each pair of groups forms one row (10 keys)
    """
    # Extract main matrix keys (exclude thumb cluster)
    main_keys = layer_keys[:30]

    # QMK layout in row-major order (left hand then right hand)
    # Build the mouseless grid row by row
    groups = []

    for row_idx in range(3):
        # Left hand: reverse the keys for this row (right-to-left reading)
        left_start = row_idx * 5
        left_keys = main_keys[left_start:left_start + 5]
        left_keys_reversed = left_keys[::-1]
        left_group = ''.join(qmk_to_mouseless_key(k) for k in left_keys_reversed)

        # Right hand: normal order (left-to-right reading)
        right_start = 15 + row_idx * 5
        right_keys = main_keys[right_start:right_start + 5]
        right_group = ''.join(qmk_to_mouseless_key(k) for k in right_keys)

        groups.append(left_group)
        groups.append(right_group)

    return ' '.join(groups)


def create_subgrid_string(grid_string: str, exclude_keys: set[str]) -> str:
    """
    Create subgrid string by removing specified keys from grid string.

    Args:
        grid_string: Full grid string
        exclude_keys: Set of keys to exclude (e.g., {"Q", "F", "V", "J", "/"})

    Returns:
        Subgrid string with excluded keys removed
    """
    result = []
    for char in grid_string:
        if char == ' ':
            result.append(' ')
        elif char not in exclude_keys:
            result.append(char)
    return ''.join(result)


def generate_mouseless_config(json_path: Path, output_path: Path) -> None:
    """
    Generate mouseless grid configuration from QMK keymap JSON.

    Args:
        json_path: Path to keymap JSON file (e.g., keymap-drawer/charybdis.json)
        output_path: Path to output YAML file
    """
    # Read QMK keymap JSON
    with open(json_path, 'r') as f:
        keymap_data = json.load(f)

    # Extract BASE layer (layer 0)
    layers = keymap_data.get('layers', [])
    if not layers:
        raise ValueError(f"No layers found in {json_path}")

    base_layer = layers[0]
    if len(base_layer) < 30:
        raise ValueError(f"BASE layer has fewer than 30 keys: {len(base_layer)}")

    # Generate grid string
    grid_string = create_grid_string(base_layer)

    # Generate subgrid string (exclude Q, F, V, J, /)
    # These keys are typically used for mouse actions in mouseless
    exclude_keys = {'Q', 'F', 'V', 'J', '/'}
    subgrid_string = create_subgrid_string(grid_string, exclude_keys)

    # Get keyboard name from JSON
    keyboard_name = keymap_data.get('keyboard', 'unknown')
    keyboard_short = keyboard_name.split('/')[-1] if '/' in keyboard_name else keyboard_name

    # Generate output YAML
    output_content = f"""# Mouseless Grid Configuration
# Generated from: {json_path.name}
# Keyboard: {keyboard_name}
# Generation date: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

grid_configs:
  - name: colemak_{keyboard_short}
    grid_defn:
      - keys: "{grid_string}"
        num_cells_x: 10
        num_cells_y: 3
    subgrid_mouse_action_keys: "{subgrid_string}"
    subgrid_dims:
      - 8
      - 3

# Usage Instructions:
# ==================
# 1. Review the generated grid keys above
# 2. Compare with your current ~/.mouseless/configs/config.yaml
# 3. If satisfied, copy the following values:
#    - keys: "{grid_string}"
#    - subgrid_mouse_action_keys: "{subgrid_string}"
# 4. Paste into your ~/.mouseless/configs/config.yaml under grid_configs
# 5. Restart mouseless or reload config (cmd+shift+r)
#
# Grid Layout Visualization (10 keys × 3 rows):
# =============================================
# Row 0 (top):    {grid_string.split()[0]} {grid_string.split()[1]}
# Row 1 (middle): {grid_string.split()[2]} {grid_string.split()[3]}
# Row 2 (bottom): {grid_string.split()[4]} {grid_string.split()[5]}
#
# Each row shows: [LEFT HAND (5 keys)] [RIGHT HAND (5 keys)]
#
# Excluded from subgrid: {', '.join(sorted(exclude_keys))}
# (These keys are typically used for mouse actions)
"""

    # Write output file
    output_path.write_text(output_content)

    print(f"✓ Generated mouseless grid configuration")
    print(f"  Input:  {json_path}")
    print(f"  Output: {output_path}")
    print(f"\n  Grid keys: {grid_string}")
    print(f"  Subgrid:   {subgrid_string}")
    print(f"\n  Review the output file and copy to ~/.mouseless/configs/config.yaml")


def main():
    """Main entry point."""
    if len(sys.argv) < 2:
        print("Usage: python scripts/generate_mouseless_grid.py <keymap-json-path>")
        print("Example: python scripts/generate_mouseless_grid.py keymap-drawer/charybdis.json")
        sys.exit(1)

    json_path = Path(sys.argv[1])
    if not json_path.exists():
        print(f"Error: File not found: {json_path}")
        sys.exit(1)

    # Output to repository root
    output_path = Path("mouseless_grid_config.yaml")

    try:
        generate_mouseless_config(json_path, output_path)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
