
#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "yuanw.h"
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

__attribute__((weak)) bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    return true;
 }

enum layers { BASE, FUN, NAV, WIN, PNT, NUM, SYM, TXT };

enum keycode_aliases {
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
    REP_TXT   = LT(TXT, KC_0),
    MAGIC = QK_AREP,

    U_RDO = SCMD(KC_Z),
    U_PST = LCMD(KC_V),
    U_CPY = LCMD(KC_C),
    U_CUT = LCMD(KC_X),
    U_UND = LCMD(KC_Z),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_let_no_bottom_row(
        XXXXXXX, KC_Q,    KC_Y,    KC_O,   KC_U,    KC_EQL,                              KC_X,    KC_L,    KC_D,    KC_W,    KC_Z,  XXXXXXX,
        KC_B,    HRM_C,   HRM_I,   HRM_A,  HRM_E,   KC_MINS,                             KC_K,    HRM_H,   HRM_T,   HRM_N,   HRM_S, KC_F,
        XXXXXXX, KC_QUOT, KC_COMM, LT(SYM,KC_DOT), KC_SCLN,  KC_SLASH,                   KC_J,    KC_M,    LT(SYM,KC_G),    KC_P,   KC_V,  XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX,
                                            ESC_WIN, SPC_NAV, MAGIC,           REP_TXT ,  R_NUM, BSPC_FUN
                                     ),
    [SYM] = LAYOUT_let_no_bottom_row(
        _______, KC_GRV,  KC_LABK, KC_RABK, KC_MINS, KC_PIPE,                           KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR, ARROW, _______,
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
        _______, _______, _______, _______, _______, _______,                           KC_PGUP, KC_HOME, KC_UP, KC_END, _______, _______,
        _______, _______, _______, _______, _______, _______,                           KC_PGDN, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,
        _______, _______, _______, _______, _______, _______,                           RDO, PST,     CPY,     CUT,    UND, _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, QK_LLCK, _______
    ),

    [WIN] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, LAG(KC_1), LAG(KC_2), LAG(KC_3), LAG(KC_4), _______,
        _______, _______, _______, _______, _______, _______,                           _______, LSG(KC_1), LSG(KC_2), LSG(KC_3), LSG(KC_4), _______,
                                            _______, _______, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    ),

    [PNT] = LAYOUT_let_no_bottom_row(
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, QK_LLCK, _______,                           _______, QK_LLCK, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            KC_BTN1, KC_BTN2, _______,         _______, KC_BTN2, KC_BTN1,
                                            _______, KC_BTN1, KC_BTN2,         _______, _______, _______
    ),

    [NUM] = LAYOUT_let_no_bottom_row(
        _______, _______, KC_7, KC_8, KC_9, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, KC_4, KC_5, KC_6, _______,                           _______, _______, _______, _______, _______, _______,
        _______, _______, KC_1, KC_2, KC_3, _______,                           _______, _______, _______, _______, _______, _______,
                                            _______, QK_LLCK, _______,         _______, _______, _______,
                                            _______, KC_0, _______,         _______, _______, _______
    ),
    [TXT] = LAYOUT_let_no_bottom_row(
        _______, KC_7, KC_8, KC_9, KC_SECRET_1, _______,                           _______, _______, _______, _______, _______, _______,
        QK_BOOT, SELWORD , SELWBAK, SELLINE, KC_SECRET_2, _______,                           _______, _______, _______, _______, _______, _______,
        _______, KC_1, KC_2, KC_3, _______, _______,                           _______, _______, _______, _______, _______, _______,
                                            QK_LLCK, KC_MINS, _______,         _______, _______, _______,
                                            _______, _______, _______,         _______, _______, _______
    )
};
// clang-format on

const custom_shift_key_t custom_shift_keys[] = {
    {SPC_NAV, KC_TAB},  // Shift SPC is tab.
    {ESC_WIN, KC_ENT},  // Shift esc is enter.
    {BSPC_FUN, KC_DEL}, // Shift BSPC is DEL
};

bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    switch (keycode) {
        case REP_TXT:
            return false; // Ignore ALTREP keys.
    }
    return true; // Other keys can be repeated.
};


void pointing_device_init_user() {
    //    charybdis_set_pointer_dragscroll_enabled(true, true);
    set_auto_mouse_layer(PNT);   // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true); // always required before the auto mouse feature will work
    //  set_auto_mouse_timeout(5000);
}


// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
#define MAGIC_STRING(str, repeat_keycode) \
  magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
  uint8_t saved_mods = 0;
  // If Caps Word is on, save the mods and hold Shift.
  if (is_caps_word_on()) {
    saved_mods = get_mods();
    register_mods(MOD_BIT_LSHIFT);
  }

  send_string_P(str);  // Send the string.
  set_last_keycode(repeat_keycode);

  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}

// The following describes the magic key functionality, where * represents the
// magic key and @ the repeat key. For example, tapping A and then the magic key
// types "ao". Most of this is coded in my `get_alt_repeat_key_keycode_user()`
// definition below.
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PN      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (like "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     & *   -> &nbsp;          (HTML code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    // keycode = get_tap_keycode(keycode);

  if (mods == MOD_BIT_LALT) {
    switch (keycode) {
      case KC_U: return A(KC_O);
      case KC_O: return A(KC_U);
      case KC_N: return A(KC_I);
      case KC_I: return A(KC_N);
    }
  } else if ((mods & ~MOD_MASK_SHIFT) == 0) {
    // This is where most of the "magic" for the MAGIC key is implemented.
    switch (keycode) {
      case KC_SPC:  // spc -> THE
      case KC_ENT:
      case KC_TAB:
      case SPC_NAV:
        return M_THE;

      // For navigating next/previous search results in Vim:
      // N -> Shift + N, Shift + N -> N.
      case KC_N:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return S(KC_N);
        }
        return KC_N;

      // Fix SFBs and awkward strokes.
      case KC_A: return KC_O;         // A -> O
      case KC_O: return KC_A;         // O -> A
      case KC_E: return KC_U;         // E -> U
      case KC_U: return KC_E;         // U -> E
      case KC_I:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_ION;  // I -> ON
        } else {
          return KC_QUOT;  // Shift I -> '
        }
      case KC_M: return M_MENT;       // M -> ENT
      case KC_Q: return M_QUEN;       // Q -> UEN
      case KC_T: return M_TMENT;      // T -> TMENT

      case KC_C: return KC_Y;         // C -> Y
      case KC_D: return KC_Y;         // D -> Y
      case KC_G: return KC_Y;         // G -> Y
      case KC_P: return KC_N;         // P -> N
      case KC_Y: return KC_P;         // Y -> P

      case KC_L: return KC_K;         // L -> K
      case KC_S: return KC_T;         // S -> T

      case KC_R: return KC_L;         // R -> L
      case KC_DOT:
        if ((mods & MOD_MASK_SHIFT) == 0) {
          return M_UPDIR;  // . -> ./
        }
        return M_NOOP;
      case KC_HASH: return M_INCLUDE;  // # -> include
      case KC_AMPR: return M_NBSP;     // & -> nbsp;
      case KC_EQL: return M_EQEQ;      // = -> ==
      case KC_RBRC: return KC_SCLN;    // ] -> ;
      case KC_AT: return SHIP_IT;      // @ -> SHIP_IT

      case KC_COMM:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return KC_EQL;  // ! -> =
        }
        return M_NOOP;
      case KC_QUOT:
        if ((mods & MOD_MASK_SHIFT) != 0) {
          return M_DOCSTR;  // " -> ""<cursor>"""
        }
        return M_NOOP;
      case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
        return M_MKGRVS;
      case KC_LABK:  // < -> - (for Haskell)
        return KC_MINS;
      case KC_SLSH:
        return KC_SLSH;  // / -> / (easier reach than Repeat)

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
        case REP_TXT:
            if (record->tap.count) {
                repeat_key_invoke(&record->event);
                return false;
            }
            break;

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
    if(record->event.pressed){
        switch(keycode){
     case SHIP_IT:
            MAGIC_STRING("Workiva/release-management-p", KC_AT);
            return false;

     case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
        }}

    return true; // Process all other keycodes normally
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // your existing macro code here.
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
