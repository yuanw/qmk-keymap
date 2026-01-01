#pragma once

// clang-format off

// Imprint (letters only no bottom row): 48 keys
// 3 rows x 12 (6L + 6R) + 12 thumb keys (6L + 6R)
// Maps 3x5 + 5 thumb to 3x6 + 12 thumb, using outer columns for extra keys
#define LAYOUT_LR(\
    k00, k01, k02, k03, k04,     k05, k06, k07, k08, k09, \
    k10, k11, k12, k13, k14,     k15, k16, k17, k18, k19, \
    k20, k21, k22, k23, k24,     k25, k26, k27, k28, k29, \
                   k30, k31, k32,     k33, k34) \
    LAYOUT_let_no_bottom_row(\
        XXXXXXX, k00, k01, k02, k03, k04,   k05, k06, k07, k08, k09, XXXXXXX, \
        k10,     k11, k12, k13, k14, k04,   k15, k16, k17, k18, k19, k29,     \
        XXXXXXX, k20, k21, k22, k23, k24,   k25, k26, k27, k28, k29, XXXXXXX, \
                            XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, \
                            k30, k31, k32,   k33, k34, XXXXXXX  \
    )

// clang-format on
