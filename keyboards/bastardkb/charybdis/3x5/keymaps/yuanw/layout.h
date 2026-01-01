#pragma once

// clang-format off

// Charybdis 3x5: 35 keys (3 rows x 10 + 5 thumb: 3L + 2R)
// LAYOUT_LR uses 3x5 + 5 thumb pattern (same as native LAYOUT)
#define LAYOUT_LR(\
    k00, k01, k02, k03, k04,     k05, k06, k07, k08, k09, \
    k10, k11, k12, k13, k14,     k15, k16, k17, k18, k19, \
    k20, k21, k22, k23, k24,     k25, k26, k27, k28, k29, \
                   k30, k31, k32,     k33, k34) \
    LAYOUT(k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, \
           k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, \
           k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, \
                          k30, k31, k32, k33, k34)

// clang-format on
