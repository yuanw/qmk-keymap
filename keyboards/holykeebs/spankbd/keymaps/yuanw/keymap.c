#include QMK_KEYBOARD_H
#include "layout.h"

#include "../../../../../yuanw.c"

#ifdef POINTING_DEVICE_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t right_report) {
    // Left TPS43: scroll only — convert Y movement to vertical scroll, suppress cursor
    left_report.v = -left_report.y / SPANKBD_SCROLL_DIVISOR;
    left_report.x = 0;
    left_report.y = 0;

    // Right TPS43: cursor only — activate PNT layer on movement
    if (abs(right_report.x) > SPANKBD_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD ||
        abs(right_report.y) > SPANKBD_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(PNT);
        }
        auto_pointer_layer_timer = timer_read();
    }

    return pointing_device_combine_reports(left_report, right_report);
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 &&
        TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= SPANKBD_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(PNT);
    }
    app_switcher_task();
}
#endif
