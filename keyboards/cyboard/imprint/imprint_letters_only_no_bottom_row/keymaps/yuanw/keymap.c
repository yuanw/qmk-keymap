
#include QMK_KEYBOARD_H
#include <cyboard.h>
#include "yuanw.h"
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

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
                                            QK_LLCK, KC_MINS, _______,         _______, _______, _______,
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
