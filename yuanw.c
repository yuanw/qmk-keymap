// Shared keymap for Charybdis 3x5 and Cyboard Imprint
// Following the pattern from getreuer-keymap

#include "yuanw.h"

#ifdef OS_DETECTION_ENABLE
#include "os_detection.h"
#endif

__attribute__((weak)) bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    return true;
}

enum layers { BASE, SYM, NAV, WIN, PNT, NUM, TXT, FUN };

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

    ESC_WIN  = LT(WIN, KC_ESC),
    SPC_NAV  = LT(NAV, KC_SPC),
    BSPC_FUN = LT(FUN, KC_BSPC),
    ENT_SYM  = LT(SYM, KC_ENT),
    R_NUM    = LT(NUM, KC_R),
    // https://getreuer.info/posts/keyboards/faqs/index.html#layer-tap-repeat-key
    REP_TXT  = LT(TXT, KC_0),

    U_RDO = SCMD(KC_Z),
    U_PST = LCMD(KC_V),
    U_CPY = LCMD(KC_C),
    U_CUT = LCMD(KC_X),
    U_UND = LCMD(KC_Z),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_LR(
        KC_Q,    KC_Y,    KC_O,    KC_U,    KC_EQL,      KC_X,    KC_L,    KC_D,    KC_W,    KC_Z,
        HRM_C,   HRM_I,   HRM_A,   HRM_E,   KC_MINS,     KC_K,    HRM_H,   HRM_T,   HRM_N,   HRM_S,
        KC_QUOT, KC_COMM, LT(SYM,KC_DOT), KC_SCLN, KC_SLASH,   KC_J,    KC_M,    LT(SYM,KC_G), KC_P, KC_V,
                          ESC_WIN, SPC_NAV, MAGIC,       REP_TXT, R_NUM
    ),

    [SYM] = LAYOUT_LR(
        KC_GRV,  KC_LABK, KC_RABK, KC_MINS, KC_PIPE,     KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR,  ARROW,
        KC_EXLM, KC_ASTR, KC_SLSH, KC_EQL,  KC_AMPR,     KC_HASH, KC_LPRN, KC_RPRN, KC_RIGHT, _______,
        KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_PERC,     KC_AT,   KC_BSLS, KC_DOT,  _______, _______,
                          _______, _______, _______,     _______, _______
    ),

    [NAV] = LAYOUT_LR(
        _______, _______, _______, _______, _______,     KC_PGUP, KC_HOME, KC_UP,   KC_END,  _______,
        _______, _______, _______, _______, _______,     KC_PGDN, KC_LEFT, KC_DOWN, KC_RIGHT, _______,
        _______, _______, _______, _______, _______,     RDO,     PST,     CPY,     CUT,     UND,
                          _______, _______, _______,     _______, QK_LLCK
    ),

    [WIN] = LAYOUT_LR(
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______, LAG(KC_1), LAG(KC_2), LAG(KC_3), LAG(KC_4),
        _______, _______, _______, _______, _______,     _______, LSG(KC_1), LSG(KC_2), LSG(KC_3), LSG(KC_4),
                          _______, _______, _______,     _______, _______
    ),

    [PNT] = LAYOUT_LR(
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
        _______, _______, _______, QK_LLCK, _______,     _______, QK_LLCK, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
                          KC_BTN1, KC_BTN2, _______,     _______, KC_BTN1
    ),

    [NUM] = LAYOUT_LR(
        _______, KC_7,    KC_8,    KC_9,    _______,     _______, _______, _______, _______, _______,
        _______, KC_4,    KC_5,    KC_6,    _______,     _______, _______, _______, _______, _______,
        _______, KC_1,    KC_2,    KC_3,    _______,     _______, _______, _______, _______, _______,
                          _______, KC_0,    _______,     _______, _______
    ),

    [TXT] = LAYOUT_LR(
        KC_7,    KC_8,    KC_9,    KC_SECRET_1, _______,  _______, _______, _______, _______, _______,
        SELWORD, SELWBAK, SELLINE, KC_SECRET_2, _______,  _______, _______, _______, _______, _______,
        KC_1,    KC_2,    KC_3,    _______, _______,      _______, _______, _______, _______, _______,
                          QK_LLCK, KC_MINS, _______,      _______, _______
    ),

    [FUN] = LAYOUT_LR(
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,
                          _______, _______, _______,     _______, _______
    ),
};
// clang-format on

const custom_shift_key_t custom_shift_keys[] = {
    {SPC_NAV, KC_TAB},   // Shift SPC is tab.
    {ESC_WIN, KC_ENT},   // Shift esc is enter.
    {BSPC_FUN, KC_DEL},  // Shift BSPC is DEL
};

bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    switch (keycode) {
        case REP_TXT:
            return false;  // Ignore ALTREP keys.
    }
    return true;  // Other keys can be repeated.
}

// MAGIC_STRING helper: if Caps Word is active, hold Shift while sending string
#define MAGIC_STRING(str, repeat_keycode) magic_send_string_P(PSTR(str), (repeat_keycode))

static void magic_send_string_P(const char *str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT_LSHIFT);
    }
    send_string_P(str);
    set_last_keycode(repeat_keycode);
    if (is_caps_word_on()) {
        set_mods(saved_mods);
    }
}

// Comprehensive magic key functionality
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    keycode = get_tap_keycode(keycode);

    if (mods == MOD_BIT_LALT) {
        switch (keycode) {
            case KC_U: return A(KC_O);
            case KC_O: return A(KC_U);
            case KC_N: return A(KC_I);
            case KC_I: return A(KC_N);
        }
    } else if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
            case KC_SPC:
            case KC_ENT:
            case KC_TAB:
            case SPC_NAV:
                return M_THE;

            case KC_N:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return S(KC_N);
                }
                return KC_N;

            // Fix SFBs and awkward strokes
            case KC_A: return KC_O;
            case KC_O: return KC_A;
            case KC_E: return KC_U;
            case KC_U: return KC_E;
            case KC_I:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_ION;
                } else {
                    return KC_QUOT;
                }
            case KC_M: return M_MENT;
            case KC_Q: return M_QUEN;
            case KC_T: return M_TMENT;

            case KC_C: return KC_Y;
            case KC_D: return KC_Y;
            case KC_G: return KC_Y;
            case KC_P: return KC_N;
            case KC_Y: return KC_P;

            case KC_L: return KC_K;
            case KC_S: return KC_T;
            case KC_R: return KC_L;

            case KC_DOT:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_UPDIR;
                }
                return M_NOOP;
            case KC_HASH: return M_INCLUDE;
            case KC_AMPR: return M_NBSP;
            case KC_EQL: return M_EQEQ;
            case KC_RBRC: return KC_SCLN;
            case KC_AT: return SHIP_IT;

            case KC_COMM:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return KC_EQL;
                }
                return M_NOOP;
            case KC_QUOT:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return M_DOCSTR;
                }
                return M_NOOP;
            case KC_GRV:
                return M_MKGRVS;
            case KC_LABK:
                return KC_MINS;
            case KC_SLSH:
                return KC_SLSH;

            case KC_PLUS:
            case KC_MINS:
            case KC_ASTR:
            case KC_PERC:
            case KC_PIPE:
            case KC_CIRC:
            case KC_TILD:
            case KC_EXLM:
            case KC_DLR:
            case KC_RABK:
            case KC_LPRN:
            case KC_RPRN:
            case KC_UNDS:
            case KC_COLN:
                return KC_EQL;
            case KC_F:
            case KC_V:
            case KC_X:
            case KC_SCLN:
            case KC_1 ... KC_0:
                return M_NOOP;
        }
    }

    switch (keycode) {
        case KC_WH_U: return KC_WH_D;
        case KC_WH_D: return KC_WH_U;
        case SELWBAK: return SELWORD;
        case SELWORD: return SELWBAK;
    }
    return KC_TRNS;
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods     = get_mods();
    const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                              | get_oneshot_mods()
#endif
    );
    const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;
    const bool    alt        = all_mods & MOD_BIT_LALT;

    // Set last key to KC_N for certain alt repeat patterns
    if (get_repeat_key_count() < 0 && (all_mods & ~MOD_MASK_SHIFT) == 0 &&
        (keycode == KC_A || keycode == KC_E || keycode == KC_I ||
         keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }

    switch (keycode) {
        case ARROW:
            if (record->event.pressed) {
                clear_weak_mods();
                clear_mods();
                SEND_STRING(alt ? (shift_mods ? "<=>"
                                              : "<->")
                                : (shift_mods ? "=>"
                                              : "->"));
                set_mods(mods);
            }
            return false;

        case REP_TXT:
            if (record->tap.count) {
                repeat_key_invoke(&record->event);
                return false;
            }
            break;

        case CPY:
            if (record->event.pressed) {
#ifdef OS_DETECTION_ENABLE
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(LCMD(KC_C));
                        return false;
                    default:
                        tap_code16(KC_COPY);
                        return false;
                }
#else
                tap_code16(LCMD(KC_C));
                return false;
#endif
            }
            break;

        case PST:
            if (record->event.pressed) {
#ifdef OS_DETECTION_ENABLE
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(LCMD(KC_V));
                        break;
                    default:
                        tap_code16(KC_PSTE);
                        break;
                }
#else
                tap_code16(LCMD(KC_V));
#endif
            }
            return false;

        case CUT:
            if (record->event.pressed) {
#ifdef OS_DETECTION_ENABLE
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(LCMD(KC_X));
                        break;
                    default:
                        tap_code16(KC_CUT);
                        break;
                }
#else
                tap_code16(LCMD(KC_X));
#endif
            }
            return false;

        case UND:
            if (record->event.pressed) {
#ifdef OS_DETECTION_ENABLE
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(LCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_UNDO);
                        break;
                }
#else
                tap_code16(LCMD(KC_Z));
#endif
            }
            return false;

        case RDO:
            if (record->event.pressed) {
#ifdef OS_DETECTION_ENABLE
                switch (detected_host_os()) {
                    case OS_MACOS:
                    case OS_IOS:
                        tap_code16(SCMD(KC_Z));
                        break;
                    default:
                        tap_code16(KC_AGIN);
                        break;
                }
#else
                tap_code16(SCMD(KC_Z));
#endif
            }
            return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case SHIP_IT:
                MAGIC_STRING("Workiva/release-management-p", KC_AT);
                return false;

            case UPDIR:
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
                return false;

            case M_THE:     MAGIC_STRING("the", KC_N); break;
            case M_ION:     MAGIC_STRING("on", KC_S); break;
            case M_MENT:    MAGIC_STRING("ent", KC_S); break;
            case M_QUEN:    MAGIC_STRING("uen", KC_C); break;
            case M_TMENT:   MAGIC_STRING("ment", KC_S); break;
            case M_UPDIR:   MAGIC_STRING("./", UPDIR); break;
        }
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
