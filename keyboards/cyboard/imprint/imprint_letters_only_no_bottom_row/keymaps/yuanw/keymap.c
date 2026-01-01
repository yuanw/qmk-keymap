// Cyboard Imprint keymap - minimal wrapper for shared yuanw.c

#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "layout.h"
#include "yuanw.c"

// Imprint-specific: auto-mouse initialization
void pointing_device_init_user(void) {
    set_auto_mouse_layer(PNT);
    set_auto_mouse_enable(true);
}
