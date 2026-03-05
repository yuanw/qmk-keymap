#pragma once
#define KEYBOARD_SPANKBD

// LAYOUT_LR: Abstraction layer for shared keymap
// Shared layout is 3x5 + 3 left thumbs + 3 right thumbs (36 keys total).
// Span has 3 thumbs per side, so RT2 is used (direct 1:1 mapping).
//
// Layout diagram:
//   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04   <- Row 0
//   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14   <- Row 1
//   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24   <- Row 2
//             LT0 LT1 LT2       RT0 RT1 RT2      <- Thumbs (all used)

#define LAYOUT_LR( \
    L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, \
              LT0, LT1, LT2,         RT0, RT1, RT2        \
) LAYOUT_split_3x5_3( \
    L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, \
              LT0, LT1, LT2,         RT0, RT1, RT2        \
)
