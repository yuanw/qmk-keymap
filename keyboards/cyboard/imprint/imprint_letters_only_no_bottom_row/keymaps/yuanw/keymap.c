
#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "yuanw.h"
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

__attribute__ ((weak))
bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
  return true;
}

enum layers {
    BASE,
    FUN,
    NAV,
    WIN,
    PNT,
    NUM,
    SYM,
    TXT
};

enum keycode_aliases {
  // Short aliases for home row mods and other tap-hold keys.
  HRM_A = LSFT_T(KC_A),
  HRM_E = LCTL_T(KC_E),
  HRM_I = LALT_T(KC_I),
  HRM_C = LGUI_T(KC_C),
  HRM_QUOT = LGUI_T(KC_QUOT),

  HRM_H = RSFT_T(KC_H),
  HRM_T = RCTL_T(KC_T),
  HRM_N = LALT_T(KC_N),
  HRM_S = RGUI_T(KC_S),
  HRM_V = RGUI_T(KC_V),

  ESC_WIN = LT(WIN, KC_ESC),
  SPC_NAV = LT(NAV, KC_SPC),
  BSPC_FUN = LT(FUN, KC_BSPC),
  ENT_SYM = LT(SYM, KC_ENT),
  R_NUM = LT(NUM, KC_R),
  //https://getreuer.info/posts/keyboards/faqs/index.html#layer-tap-repeat-key
  REP_SYM = LT(SYM, KC_0),
  MAGIC_TXT = LT(TXT, KC_1),

  U_RDO= SCMD(KC_Z),
  U_PST= LCMD(KC_V),
  U_CPY= LCMD(KC_C),
  U_CUT= LCMD(KC_X),
  U_UND= LCMD(KC_Z),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_let_no_bottom_row(
        XXXXXXX, KC_Z,    KC_Y,    KC_U,   KC_O,    KC_SCLN,                             KC_Q,    KC_L,    KC_D,    KC_P,    KC_X,  XXXXXXX,
        KC_W,    HRM_C,   HRM_I,   HRM_E,  HRM_A,   KC_COMM,                             KC_K,    HRM_H,   HRM_T,   HRM_N,   HRM_N, HRM_S,
        XXXXXXX, KC_QUOT, KC_MINS, KC_EQL, KC_DOT,  KC_SLASH,                            KC_J,    KC_M,    KC_G,    KC_B,    KC_V,  XXXXXXX,
                                            SPC_NAV, ENT_SYM, XXXXXXX,           XXXXXXX, REP_SYM, R_NUM,
                                            MAGIC_TXT, ESC_WIN, XXXXXXX,         XXXXXXX,  BSPC_FUN,  KC_APP
                                     ),
    [SYM] = LAYOUT_let_no_bottom_row(
        QK_BOOT, KC_GRV,  KC_LABK, KC_RABK, KC_MINS, KC_PIPE,                           KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR, ARROW, _______,
        _______, KC_EXLM, KC_ASTR, KC_SLSH, KC_EQL,  KC_AMPR,                           KC_HASH, KC_LPRN, KC_RPRN, KC_RIGHT, _______, _______,
        _______, KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_PERC,                           KC_AT,   KC_BSLS, KC_DOT, _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [FUN] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [NAV] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [WIN] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [PNT] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [NUM] = LAYOUT_let_no_bottom_row(
        _______, KC_7, KC_8, KC_9, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, KC_4, KC_5, KC_6, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, KC_1, KC_2, KC_3, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            KC_0, QK_LLCK, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),
    [TXT] = LAYOUT_let_no_bottom_row(
        _______, KC_7, KC_8, KC_9, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, KC_4, KC_5, KC_6, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, KC_1, KC_2, KC_3, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            QK_LLCK, KC_MINS, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    )
};

const custom_shift_key_t custom_shift_keys[] = {
    {SPC_NAV , KC_TAB}, // Shift SPC is tab.
    {ESC_WIN , KC_ENT}, // Shift esc is enter.
    {BSPC_FUN, KC_DEL}, //Shift BSPC is DEL
};

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    switch (keycode) {
        case REP_SYM:
        case ALTREP2:
        case ALTREP3:
            return false;  // Ignore ALTREP keys.
    }
    return true;  // Other keys can be repeated.
};

static void process_altrep2(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case KC_A:
        case RCTL_T(KC_A):
          SEND_STRING("tion");
          break;
        case LALT_T(KC_I): SEND_STRING("tion"); break;
        case LALT_T(KC_S): SEND_STRING("sion"); break;
        case LSFT_T(KC_T): SEND_STRING("heir"); break;
        case KC_Y: SEND_STRING("ou"); break;
        case KC_W: SEND_STRING("hich"); break;
        case KC_AT: SEND_STRING("Workiva/release-management-p"); break;
        case KC_C: SEND_STRING("ontent management"); break;
    }
};

static void process_altrep3(uint16_t keycode, uint8_t mods) {
    switch (keycode) {
        case RCTL_T(KC_A): SEND_STRING("bout"); break;
        case LALT_T(KC_I): SEND_STRING("ng"); break;
        case LALT_T(KC_S): SEND_STRING("tate"); break;
        case LSFT_T(KC_T): SEND_STRING("here"); break;
        case KC_W: SEND_STRING("ould"); break;
        case KC_AT: SEND_STRING("rmconsole-wf"); break;
        case KC_E: SEND_STRING("specially");break;
        case KC_C: SEND_STRING("ontent-management-service"); break;
    }
};


bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  const uint8_t mods = get_mods();
  const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                        | get_oneshot_mods()
#endif  // NO_ACTION_ONESHOT
  );
  const uint8_t shift_mods = all_mods & MOD_MASK_SHIFT;
  const bool alt = all_mods & MOD_BIT_LALT;
    switch (keycode) {
        case ARROW:
            if (record->event.pressed) {
              clear_weak_mods();
              clear_mods();
               SEND_STRING(alt ? (shift_mods
                                    ? "<=>"     // <=>
                                    : "<->")    // <->
                                 : (shift_mods
                                    ? "=>"     // =>
                                    : "->"));     // ->
              set_mods(mods);
            }
          return false;
        case REP_SYM:
            if (record->tap.count) {
                    repeat_key_invoke(&record->event);
                    return false;
            }
            break;
        case MAGIC_TXT:
            if (record->tap.count) {
                process_altrep2(get_last_keycode(), get_last_mods());
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
