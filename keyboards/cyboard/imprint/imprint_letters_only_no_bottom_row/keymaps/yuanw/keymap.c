#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "layout.h"

#include "yuanw.c"

void pointing_device_init_user(void) {
    set_auto_mouse_layer(PNT);
    set_auto_mouse_enable(true);
}

// Combos (keyboard-specific due to physical layout)
const uint16_t PROGMEM test_combo1[] = {R_NUM, BSPC_FUN, COMBO_END};
const uint16_t PROGMEM test_combo2[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM test_combo3[] = {KC_M, LR_G, COMBO_END};
const uint16_t PROGMEM test_combo4[] = {ESC_WIN, SPC_NAV, COMBO_END};
combo_t                key_combos[]  = {
    COMBO(test_combo1, QK_REP),
    COMBO(test_combo2, QK_REP),
    COMBO(test_combo3, QK_AREP),
    COMBO(test_combo4, QK_AREP),
};
