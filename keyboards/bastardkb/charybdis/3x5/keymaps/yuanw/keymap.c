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

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE,
    LAYER_HDGOLD,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_MEDIA,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
};

enum keycode_aliases {
    // The "magic" key is the Alternate Repeat Key.
    MAGIC = QK_AREP,
    // Short aliases for home row mods and other tap-hold keys.
    HRM_E    = LSFT_T(KC_E),
    HRM_A    = LCTL_T(KC_A),
    HRM_I    = LALT_T(KC_I),
    HRM_C    = LGUI_T(KC_C),
    HRM_QUOT = LGUI_T(KC_QUOT),

    HRM_H = RSFT_T(KC_H),
    HRM_T = RCTL_T(KC_T),
    HRM_N = LALT_T(KC_N),
    HRM_S = RGUI_T(KC_S),
    HRM_V = RGUI_T(KC_V),

    ESC_MED = LT(LAYER_MEDIA, KC_ESC),
    SPC_NAV = LT(LAYER_NAVIGATION, KC_SPC),
    TAB_FUN = LT(LAYER_FUNCTION, KC_BSPC),
    ENT_SYM = LT(LAYER_SYMBOLS, KC_ENT),
    E_NUM   = LT(LAYER_NUMERAL, KC_E),
    ADEPT   = DF(LAYER_BASE),
    HD_GOLD = DF(LAYER_HDGOLD),
    T_NAV   = LT(LAYER_NAVIGATION, KC_T),
    SPC_NUM = LT(LAYER_NUMERAL, KC_SPC),
    R_NUM   = LT(LAYER_NUMERAL, KC_R),
    // https://getreuer.info/posts/keyboards/faqs/index.html#layer-tap-repeat-key
    REP_SYM = LT(LAYER_SYMBOLS, KC_0),

    U_RDO = SCMD(KC_Z),
    U_PST = LCMD(KC_V),
    U_CPY = LCMD(KC_C),
    U_CUT = LCMD(KC_X),
    U_UND = LCMD(KC_Z),
    LR_DOT = LT(LAYER_SYMBOLS, KC_DOT),
    LR_G = LT(LAYER_SYMBOLS, KC_G),
};

// clang-format off
 /** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

/** \brief adept layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,     KC_Y,    KC_O,    KC_U,   KC_EQL,                             KC_X, KC_L, KC_D, KC_P,  KC_Z, \
       HRM_C,    HRM_I,   HRM_A,   HRM_E,  KC_MINS,                            KC_K, HRM_H, HRM_T,  HRM_N,  HRM_S,      \
       KC_QUOT,  KC_COMM, LR_DOT,  KC_SLASH,  KC_SCLN,                         KC_J, KC_M,  LR_G,    KC_F,    KC_V, \
                       ESC_MED, SPC_NAV, QK_AREP,    ENT_SYM, R_NUM



/*
 * Layers used on the Charybdis Nano.
 *
 * These layers started off heavily inspired by the Miryoku layout, but trimmed
 * down and tailored for a stock experience that is meant to be fundation for
 * further personalization.
 *
 * See https://github.com/manna-harbour/miryoku for the original layout.
 */

/**
 * \brief Function layer.
 *
 * Secondary right-hand layer has function keys mirroring the numerals on the
 * primary layer with extras on the pinkie column, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
  * from the base layer to enable auto-repeat.
 */
#define LAYOUT_LAYER_FUNCTION                                                                 \
    _______________DEAD_HALF_ROW_______________, XXXXXXX,   KC_LCBR,   KC_BSLS,    KC_RCBR,  MAGIC,  \
    ______________HOME_ROW_GACS_L______________, KC_SCRL,   LAG(KC_1),   LAG(KC_2), LAG(KC_3),  LAG(KC_4), \
    _______________DEAD_HALF_ROW_______________, KC_PAUS,   LSG(KC_1),   LSG(KC_2), LSG(KC_3),  LSG(KC_4), \
                      XXXXXXX, XXXXXXX, _______, XXXXXXX, KC_TAB


/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accomodate the left- and right-hand trackball.
 */
#define LAYOUT_LAYER_MEDIA                                                                    \
    KC_SECRET_1,RGB_RMOD, RGB_TOG, RGB_MOD, ADEPT, HD_GOLD, KC_LBRC, KC_SLSH, KC_RBRC, XXXXXXX, \
    KC_MPRV, KC_VOLD, KC_MUTE, KC_VOLU, KC_MNXT, QK_CAPS_WORD_TOGGLE, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
    KC_SECRET_2, XXXXXXX, XXXXXXX, EE_CLR,  QK_BOOT, QK_BOOT, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
                      _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_TAB


/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                  \
    XXXXXXX,  XXXXXXX, QK_LLCK, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, QK_LLCK,  XXXXXXX, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______, DRGSCRL, KC_BTN2, KC_BTN3, XXXXXXX, XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, \
                      XXXXXXX, KC_BTN1, XXXXXXX, KC_BTN3, KC_BTN1

/**
 * \brief Navigation layer.
 *
 * Primary right-hand layer (left home thumb) is navigation and editing. Cursor
 * keys are on the home position, line and page movement below, clipboard above,
 * caps lock and insert on the inner column. Thumb keys are duplicated from the
 * base layer to avoid having to layer change mid edit and to enable auto-repeat.
 */
#define LAYOUT_LAYER_NAVIGATION                                                               \
    _______________DEAD_HALF_ROW_______________, XXXXXXX, KC_LPRN,  KC_UP, KC_RPRN, MAGIC, \
    ______________HOME_ROW_GACS_L______________, KC_CAPS, KC_LEFT, KC_DOWN,  KC_UP,    KC_RGHT, \
    _______________DEAD_HALF_ROW_______________, RDO,     PST,     CPY,      CUT,      UND, \
                      XXXXXXX, _______, XXXXXXX, QK_LLCK , KC_CAPS


/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 * `KC_DOT` is duplicated from the base layer.
 */
#define LAYOUT_LAYER_NUMERAL                                                                  \
    XXXXXXX,    KC_7,    KC_8,    KC_9, KC_RBRC, _______________DEAD_HALF_ROW_______________, \
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, ______________HOME_ROW_GACS_R______________, \
     KC_GRV,    KC_1,    KC_2,    KC_3, KC_BSLS, _______________DEAD_HALF_ROW_______________, \
        QK_LLCK,    KC_0, KC_MINS,                XXXXXXX, _______



#define LAYOUT_LAYER_SYMBOLS                  \
    KC_GRV , KC_LABK, KC_RABK, KC_MINS, KC_PIPE,   KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR , ARROW ,\
    KC_EXLM, KC_ASTR, KC_SLSH, KC_EQL, KC_AMPR,   KC_HASH, KC_LPRN, KC_RPRN, KC_CAPS, CW_TOGG,\
    KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_PERC,   KC_AT  , KC_BSLS, KC_DOT, SELWBAK , SELWORD,\
                       XXXXXXX, _______, XXXXXXX,  _______ , _______




#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(LAYOUT_LAYER_BASE),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),

};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
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
const uint16_t PROGMEM enthium_q_combo[] = {ENT_SYM, R_NUM, COMBO_END};

combo_t key_combos[] = {
    [FD_B] = COMBO(b_combo, KC_B), [LU_J] = COMBO(j_combo, KC_J),
    [MP_Q] = COMBO(q_combo, KC_Q), [COMMSCLN_K] = COMBO(k_combo, KC_K),
    [SCLNDOT_Z] = COMBO(z_combo, KC_Z), [LEFT_QUESTION] = COMBO(left_combo, KC_QUESTION), [CF_X] = COMBO(x_combo, KC_X), [UY_L] = COMBO(l_combo, KC_L), [UO_W] = COMBO(enthium_w_combo, KC_W), [DP_F] = COMBO(enthium_f_combo, KC_F), [LD_Q] = COMBO(enthium_q_combo, QK_REP),

};

const custom_shift_key_t custom_shift_keys[] = {
    {SPC_NAV, KC_TAB}, // Shift SPC is tab.
    {ESC_MED, KC_ENT}, // Shift esc is enter.
    {TAB_FUN, KC_DEL}, // Shift BSPC is DEL
};

// Use ALTREP2 and ALTREP3 in your layout...

bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    switch (keycode) {
        case REP_SYM:
        case ALTREP2:
        case ALTREP3:
            return false; // Ignore ALTREP keys.
    }

    return true; // Other keys can be repeated.
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    /* if ((mods & MOD_MASK_CTRL)) {  // Was Ctrl held? */
    /*     switch (keycode) { */
    /*         case KC_Y: return C(KC_Z);  // Ctrl + Y reverses to Ctrl + Z. */
    /*         case KC_Z: return C(KC_Y);  // Ctrl + Z reverses to Ctrl + Y. */
    /*     } */
    /* } */
    switch (keycode) {
        case KC_LPRN:
            return KC_RPRN;
        case KC_RPRN:
            return KC_LPRN;
    }

    return KC_TRNS; // Defer to default definitions.
}

static void process_altrep2(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_A:
        case RCTL_T(KC_A):
            SEND_STRING("tion");
            break;
        case LALT_T(KC_I):
            SEND_STRING("tion");
            break;
        case LALT_T(KC_S):
            SEND_STRING("sion");
            break;
        case LSFT_T(KC_T):
            SEND_STRING("heir");
            break;
        case KC_Y:
            SEND_STRING("ou");
            break;
        case KC_W:
            SEND_STRING("hich");
            break;
        case KC_AT:
            SEND_STRING("Workiva/release-management-p");
            break;
        case KC_C:
            SEND_STRING("ontent management");
            break;
    }
}

static void process_altrep3(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case RCTL_T(KC_A):
            SEND_STRING("bout");
            break;
        case LALT_T(KC_I):
            SEND_STRING("ng");
            break;
        case LALT_T(KC_S):
            SEND_STRING("tate");
            break;
        case LSFT_T(KC_T):
            SEND_STRING("here");
            break;
        case KC_W:
            SEND_STRING("ould");
            break;
        case KC_AT:
            SEND_STRING("rmconsole-wf");
            break;
        case E_NUM:
            SEND_STRING("specially");
            break;
        case KC_C:
            SEND_STRING("ontent-management-service");
            break;
    }
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods     = get_mods();
    const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                              | get_oneshot_mods()
#endif // NO_ACTION_ONESHOT
    );
    const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;
    const bool    alt        = all_mods & MOD_BIT_LALT;
    switch (keycode) {
        case ARROW:
            if (record->event.pressed) {
                clear_weak_mods();
                clear_mods();
                SEND_STRING(alt ? (shift_mods ? "<=>"   // <=>
                                              : "<->")  // <->
                                : (shift_mods ? "=>"    // =>
                                              : "->")); // ->
                set_mods(mods);
            }
            return false;
        case REP_SYM:
            if (record->tap.count) {
                if (alt) {
                    process_altrep2(get_last_keycode(), get_last_mods());
                } else {
                    repeat_key_invoke(&record->event);
                }
                return false;
            }

            break;
        case ALTREP2:
            if (record->event.pressed) {
                process_altrep2(get_last_keycode(), get_last_mods());
            }
            return false;
        case ALTREP3:
            if (record->event.pressed) {
                process_altrep3(get_last_keycode(), get_last_mods());
            }
            return false;
        case CPY:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_C));
                        return false;
                    default:
                        tap_code16(KC_COPY);
                        return false;
                }
            }
        case PST:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_V));
                        break;
                    default:
                        tap_code16(KC_PSTE);
                        break;
                }
            }
            return false;
        case CUT:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_X));
                        break;
                    default:
                        tap_code16(KC_CUT);
                        break;
                }
            }
            return false;
        case UND:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_UNDO);
                        break;
                }
            }
            return false;
        case RDO:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS: // On Mac, set default layer to BASE_MAC.
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_AGIN);
                        break;
                }
            }
            return false;
    }
    return true; // Process all other keycodes normally
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // your existing macro code here.
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
