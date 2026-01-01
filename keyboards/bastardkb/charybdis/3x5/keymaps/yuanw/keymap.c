// Charybdis 3x5 keymap - minimal wrapper for shared yuanw.c

#include QMK_KEYBOARD_H
#include "layout.h"
#include "yuanw.c"

// Charybdis-specific: trackball auto-pointer layer
#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#include "timer.h"

static uint16_t auto_pointer_layer_timer = 0;

#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#endif

#ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#endif

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD ||
        abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(PNT);
#ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#endif
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 &&
        TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(PNT);
#ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#endif
    }
}
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, PNT));
    return state;
}
#endif

// Charybdis-specific: combos
#ifdef COMBO_ENABLE
enum combos { FD_B, LU_J, MP_Q, COMMSCLN_K, SCLNDOT_Z, LEFT_QUESTION, CF_X, UY_L, UO_W, DP_F, LD_Q, COMBO_LENGTH };

uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM b_combo[]         = {KC_F, KC_D, COMBO_END};
const uint16_t PROGMEM j_combo[]         = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM q_combo[]         = {KC_M, KC_P, COMBO_END};
const uint16_t PROGMEM k_combo[]         = {KC_COMM, KC_SCLN, COMBO_END};
const uint16_t PROGMEM z_combo[]         = {KC_SCLN, KC_DOT, COMBO_END};
const uint16_t PROGMEM left_combo[]      = {KC_W, KC_M, COMBO_END};
const uint16_t PROGMEM x_combo[]         = {KC_C, KC_F, COMBO_END};
const uint16_t PROGMEM l_combo[]         = {KC_U, KC_Y, COMBO_END};
const uint16_t PROGMEM enthium_w_combo[] = {KC_U, KC_O, COMBO_END};
const uint16_t PROGMEM enthium_f_combo[] = {KC_D, KC_P, COMBO_END};
const uint16_t PROGMEM enthium_q_combo[] = {KC_L, KC_D, COMBO_END};

combo_t key_combos[] = {
    [FD_B]          = COMBO(b_combo, KC_B),
    [LU_J]          = COMBO(j_combo, KC_J),
    [MP_Q]          = COMBO(q_combo, KC_Q),
    [COMMSCLN_K]    = COMBO(k_combo, KC_K),
    [SCLNDOT_Z]     = COMBO(z_combo, KC_Z),
    [LEFT_QUESTION] = COMBO(left_combo, KC_QUESTION),
    [CF_X]          = COMBO(x_combo, KC_X),
    [UY_L]          = COMBO(l_combo, KC_L),
    [UO_W]          = COMBO(enthium_w_combo, KC_W),
    [DP_F]          = COMBO(enthium_f_combo, KC_F),
    [LD_Q]          = COMBO(enthium_q_combo, KC_Q),
};
#endif // COMBO_ENABLE
