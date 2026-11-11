# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a QMK userspace repository containing custom keymaps for multiple split keyboards:
- **Cyboard Imprint** (3-row, no bottom row) - primary keyboard
- **BastardKB Charybdis 3x5** - trackball-equipped split keyboard
- **BastardKB Dilemma 3x5+3 (trackball)** - RP2040 split, one encoder per half
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
just build dilemma      # BastardKB Dilemma 3x5+3 trackball
just build spankbd      # Holykeebs Span (builds TWO uf2 files: left and right)

# Flash firmware to keyboard
just flash imprint
just flash charybdis
just flash dilemma
just flash spankbd left   # Flash left half
just flash spankbd right  # Flash right half

# Generate keymap visualization
just keymap charybdis   # Outputs charybdis.svg
just keymap dilemma     # Outputs dilemma.svg
just keymap imprint     # Outputs imprint.svg
just keymap spankbd     # Outputs spankbd.svg

# Generate mouseless grid configuration (from keymap)
just mouseless          # Outputs mouseless_grid_config.yaml

# Generate + open the browser keymap trainer (practice / drill / explore)
just trainer charybdis   # Outputs keymap-drawer/charybdis_trainer.html
just serve               # Serve existing pages only: index of all boards (generate_trainer.py --index)
just serve dilemma       # Serve one board, building it first if missing
python scripts/test_generate_trainer.py   # self-check for the trainer generator

# Refresh the committed board geometry the trainer builds against
just layouts

# The trainer site as a package (index page + one page per board)
nix build .#trainer && ./result/bin/keymap-trainer [target]

# Format C code
just format
```

## Architecture

### Git Submodules
- `charybdis/` - BastardKB QMK fork (main branch)
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

### Trainer
`just trainer <target>` regenerates the keymap SVG, then `scripts/generate_trainer.py`
bakes the processed keymap-drawer YAML (`keymap-drawer/<target>_output.yaml`) plus the
QMK geometry into a dependency-free page. It reads key presses from browser keydown
events, so layers are inferred from the typed character, not from firmware state.
What a key taps out (`r` = tap `R_NUM`) and the combos (`b` = `;`+`.`, `w` = `m`+`g`) are
read from the raw `keymap-drawer/<target>.json` and `yuanw.c` — the drawn labels lack them.
Geometry comes from `keymap-drawer/<target>_layout.json` — committed copies of the
keyboard `layouts` block (`just layouts`, `scripts/extract_layout.py`) — so the pages
and `nix build .#trainer` need no submodules; the test script fails if a copy goes stale.
`scripts/serve_trainer.py` opens it at `http://127.0.0.1:8765/...` (LibreWolf's sandbox
refuses to read files outside the user dirs, so `file://` will not load); the server is
reused between runs and exits after 30 idle minutes. Set `TRAINER_ROOT` to serve another
directory (the nix package does). Practice text comes from `WORDS`/`LEVELS` in the
generator: word list plus optional punctuation, numbers and code symbols, so each level
forces different layers.

Limits: only characters and modifiers reach the page, so keys that send nothing
(held MO/LT, macros, mouse, DPI) are drawn but never drilled, and combos are judged
by their output character. Prompts are lowercase — the page reads `event.key`
lowercased. Follow-ups (finger hints) live in README.org.

## BastardKB (Charybdis / Dilemma) Notes

- The fork's `main` branch is the current line; keyboard targets carry a board
  revision suffix: `bastardkb/charybdis/3x5/splinktegrated_rev1` (rev3 targets
  exist for newer controllers). Dilemma targets: `bastardkb/dilemma/3x5_3_trackball`
  (PMW3360 ball), `3x5_3` (Cirque trackpad), `3x5_3_procyon` (MaxTouch digitizer).
- On `main` the keyboard-level code (`charybdis.c`, `dilemma.c`) is an empty stub —
  the DPI / sniping / drag-scroll / auto-mouse-layer helpers moved out to the
  `modules/bastardkb` pointing module, which is not vendored here. The trackball
  behaviour both boards need (auto PNT layer on motion, sniping CPI while PNT is
  up, `DPI_UP`/`DPI_DOWN` keys) lives in the `YUANW_BKB_POINTING` block of `yuanw.c`.
- The ball is on the right half (`POINTING_DEVICE_RIGHT`); `pointing_device_set_cpi()`
  on the master is synced to that side by QMK.
- `just format` collapses the multi-line `LAYOUT_LR` macros in layout.h — wrap them
  in `// clang-format off` / `on` (done in the BastardKB layout.h files) or review
  the diff after running it.

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
