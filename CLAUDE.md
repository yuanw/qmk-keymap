# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a QMK userspace repository containing custom keymaps for split ergonomic keyboards with trackball support. The repository manages keymaps for two keyboards:
- **Charybdis 3x5** (BastardKB) - split keyboard with integrated trackball
- **Cyboard Imprint** (letters only, no bottom row variant) - split keyboard

## Build Commands

The project uses `just` as a command runner. Nix flake provides the development environment.

```bash
# Enter dev shell (provides qmk, just, clang-tools, keymap-drawer)
direnv allow  # or: nix develop

# Initialize submodules (required first time)
just init

# Build firmware
just build charybdis
just build imprint

# Flash firmware
just flash charybdis
just flash imprint

# Generate keymap visualization
just keymap      # for imprint
just keymap2     # for charybdis
```

## Repository Structure

- `keyboards/` - Contains keymap definitions
  - `bastardkb/charybdis/3x5/keymaps/yuanw/` - Charybdis keymap
  - `cyboard/imprint/imprint_letters_only_no_bottom_row/keymaps/yuanw/` - Imprint keymap
- `charybdis/` - Git submodule: BastardKB QMK fork
- `imprint/` - Git submodule: Cyboard Vial-QMK fork
- `modules/getreuer/` - Git submodule: QMK modules from getreuer (achordion, custom_shift_keys, select_word, tap_flow, etc.)

## Keymap Architecture

Each keymap follows a similar structure:
- `keymap.c` - Main keymap with layer definitions, combos, and custom key handling
- `yuanw.h` - Custom keycode enum definitions
- `config.h` - Keyboard-specific configuration
- `rules.mk` - Feature flags (OS_DETECTION, COMBO, CAPS_WORD, REPEAT_KEY, etc.)
- `secrets.c` - Secret macros (not committed)

Key features used:
- **Home row mods** (GACS: GUI, Alt, Ctrl, Shift)
- **Layer-tap keys** (e.g., `LT(LAYER_NAVIGATION, KC_SPC)`)
- **Combos** for infrequent letters (B, J, Q, K, Z, X, etc.)
- **Custom shift keys** (e.g., Shift+Space = Tab)
- **Repeat/Alt-repeat keys** with contextual behavior (ALTREP2, ALTREP3)
- **Auto pointer layer** - enables pointer layer when trackball moves
- **OS detection** - adapts copy/paste/undo shortcuts per OS

## Layers

The keymap uses these layers:
- `LAYER_BASE` - Adept layout (custom alpha arrangement)
- `LAYER_HDGOLD` - Enthium layout (alternative alpha)
- `LAYER_NAVIGATION` - Arrow keys, clipboard
- `LAYER_NUMERAL` - Number pad
- `LAYER_SYMBOLS` / `LAYER_SYMBOLS2` - Symbols
- `LAYER_FUNCTION` - Function keys, window management
- `LAYER_MEDIA` - Media controls, RGB, boot/reset
- `LAYER_POINTER` - Mouse buttons, DPI controls

## Code Formatting

```bash
clang-format -i <file> --style file
# or use treefmt via nix
```
