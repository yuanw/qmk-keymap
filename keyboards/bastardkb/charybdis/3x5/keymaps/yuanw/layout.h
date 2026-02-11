#pragma once

#define KEYBOARD_CHARYBDIS

// Charybdis-specific pointing defines
#ifdef POINTING_DEVICE_ENABLE
#    define CHARYBDIS_AUTO_SNIPING_ON_LAYER PNT
#    define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#endif

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif

// LAYOUT_LR: Abstraction layer for shared keymap
// Charybdis 3x5 is the base layout: 5 cols per side, 3 left thumbs, 2 right thumbs
// Direct mapping to LAYOUT
//
// Layout diagram:
//   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04   <- Row 0
//   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14   <- Row 1
//   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24   <- Row 2
//             LT0 LT1 LT2       RT0 RT1          <- Thumbs (3 left, 2 right)

#define LAYOUT_LR( \
    L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, \
              LT0, LT1, LT2,         RT0, RT1            \
) LAYOUT( \
    L00, L01, L02, L03, L04,    R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,    R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,    R20, R21, R22, R23, R24, \
              LT0, LT1, LT2,         RT0, RT1            \
)
