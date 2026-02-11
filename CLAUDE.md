# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a QMK userspace repository containing custom keymaps for multiple split keyboards:
- **Cyboard Imprint** (3-row, no bottom row) - primary keyboard
- **BastardKB Charybdis 3x5** - trackball-equipped split keyboard
- **Svalboard**

The layout is based on Enthium with home row mods and Miryoku-inspired layers.

## Build Commands

```bash
# Initialize submodules (required first time)
just init

# Build firmware for a keyboard
just build imprint      # Cyboard Imprint
just build charybdis    # BastardKB Charybdis 3x5

# Flash firmware to keyboard
just flash imprint
just flash charybdis

# Generate keymap visualization
just keymap             # For imprint - outputs imprint.svg

# Format C code
just c-format
```

## Architecture

### Git Submodules
- `charybdis/` - BastardKB QMK fork (bkb-master branch)
- `imprint/` - Cyboard Vial-QMK fork (cyboard branch)
- `svalboard/` - Svalboard Vial-QMK fork
- `modules/getreuer/` - QMK community modules (Achordion, custom shift keys, select word, etc.)

### Keymap Structure
Keymaps are in `keyboards/<vendor>/<keyboard>/keymaps/yuanw/`:
- `keymap.c` - Main keymap with layers and key processing
- `keymap.json` - QMK modules configuration
- `config.h` - Keyboard-specific configuration
- `yuanw.h` - Custom keycode definitions
- `secrets.c` - Secret strings (gitignored)

### Key Features
- Home row mods (CIEA on left, HTNS on right for Enthium layout)
- Magic key (QK_AREP) for SFB removal and common n-grams
- Layer-tap for space (NAV), backspace (FUN), R (NUM), escape (WIN)
- Custom shift keys (space->tab, escape->enter, backspace->delete)
- OS detection for cross-platform copy/paste/undo
- Combos for repeat key access

### Layers
BASE, SYM, NAV, WIN, PNT (pointer/mouse), NUM, TXT, FUN, REP

## Development Environment

Uses Nix flake for development dependencies. Enter with `nix develop` or direnv.

Required tools: `qmk`, `just`, `clang-format`, `keymap` (keymap-drawer)
