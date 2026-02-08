#pragma once

#define KEYBOARD_IMPRINT

// LAYOUT_LR: Abstraction layer for shared keymap
// Base layout is 3x5 with 3 left thumbs and 2 right thumbs (Charybdis style)
// Imprint has extra columns and thumb keys - wire them here
//
// Shared LAYOUT_LR (3x5 + 3/2 thumbs):
//   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04   <- Row 0
//   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14   <- Row 1
//   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24   <- Row 2
//             LT0 LT1 LT2       RT0 RT1          <- Thumbs
//
// Imprint LAYOUT_let_no_bottom_row (6 cols + 2x3 thumbs):
//   X   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04 X    <- Row 0 (add outer cols)
//   X   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14 X    <- Row 1 (add outer cols)
//   X   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24 X    <- Row 2 (add outer cols)
//                 X   X   X         X   X   X           <- Top thumbs (extra row)
//               LT0 LT1 LT2         X RT0 RT1            <- Bottom thumbs (add RT2)

#define LAYOUT_LR( \
    L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, \
              LT0, LT1, LT2,         RT0, RT1            \
) LAYOUT_let_no_bottom_row( \
    XXXXXXX, L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, XXXXXXX, \
    KC_B,    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, KC_W,    \
    XXXXXXX, L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, XXXXXXX, \
                  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, \
                  LT0,     LT1,     LT2,        QK_AREP,     RT0,     RT1 \
)
