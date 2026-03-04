# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a QMK userspace repository containing custom keymaps for multiple split keyboards:
- **Cyboard Imprint** (3-row, no bottom row) - primary keyboard
- **BastardKB Charybdis 3x5** - trackball-equipped split keyboard
- **Holykeebs Span** (spankbd) - dual TPS43 touchpad split keyboard
- **Svalboard**

The layout is based on Enthium with home row mods and Miryoku-inspired layers.

## Build Commands

```bash
# Initialize submodules (required first time)
just init

# Build firmware for a keyboard
just build imprint      # Cyboard Imprint
just build charybdis    # BastardKB Charybdis 3x5
just build spankbd      # Holykeebs Span (builds TWO uf2 files: left and right)

# Flash firmware to keyboard
just flash imprint
just flash charybdis
just flash spankbd left   # Flash left half
just flash spankbd right  # Flash right half

# Generate keymap visualization
just keymap charybdis   # Outputs charybdis.svg
just keymap imprint     # Outputs imprint.svg
just keymap spankbd     # Outputs spankbd.svg

# Generate mouseless grid configuration (from keymap)
just mouseless          # Outputs mouseless_grid_config.yaml

# Format C code
just format
```

## Architecture

### Git Submodules
- `charybdis/` - BastardKB QMK fork (bkb-master branch)
- `imprint/` - Cyboard Vial-QMK fork (cyboard branch)
- `holykeebs_qmk/` - Holykeebs QMK fork (main branch)
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

## Holykeebs Span (spankbd) Notes

### Build system quirks (holykeebs QMK fork)
The holykeebs QMK fork does **not** auto-convert make variables to preprocessor defines the way standard QMK does. Specifically:
- `SPLIT_POINTING_ENABLE = yes` and `POINTING_DEVICE_COMBINED = yes` in `rules.mk` do **not** generate `-D` flags automatically
- Fix: define them in `config.h` directly (`#define SPLIT_POINTING_ENABLE`, `#define POINTING_DEVICE_COMBINED`)
- `SERIAL_DRIVER = vendor` **does** work from `rules.mk` and is required for RP2040 USART serial (without it the bitbang driver is selected and `SOFT_SERIAL_PIN` errors occur)

### Dual TPS43 firmware
The Span has one TPS43 per side. Each half needs its own firmware:
- `just build spankbd` compiles both: `holykeebs_spankbd_yuanw_left.uf2` and `holykeebs_spankbd_yuanw_right.uf2`
- Right-side firmware has `AZOTEQ_IQS5XX_ROTATION_180` (TPS43 is physically mirrored); left does not
- `-e SIDE=left|right` is passed to `qmk compile/flash` to differentiate; handled in `rules.mk` via `ifeq ($(strip $(SIDE)), right)`
- No `users/holykeebs/` code is used — driver (`azoteq_iqs5xx`) is QMK core

### Reference build for debugging
To compare compiler flags against the known-working holykeebs keymap:
```bash
cd holykeebs_qmk
make holykeebs/spankbd:hk POINTING_DEVICE=tps43_tps43 SIDE=left USER_NAME=holykeebs -j8
cat .build/obj_holykeebs_spankbd_hk/cflags.txt | tr ' ' '\n' | grep -E "SPLIT|COMBINED|SERIAL|AZOTEQ"
```

## Development Environment

Uses Nix flake for development dependencies. Enter with `nix develop` or direnv.

Required tools: `qmk`, `just`, `clang-format`, `keymap` (keymap-drawer)
