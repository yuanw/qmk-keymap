#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "layout.h"

#include "yuanw.c"

void matrix_scan_user(void) {
    app_switcher_task();
}

void pointing_device_init_user(void) {
    set_auto_mouse_layer(PNT);
    set_auto_mouse_enable(true);
}
