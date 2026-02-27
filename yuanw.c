// Shared keymap logic for Cyboard Imprint and BastardKB Charybdis
// Include layout.h before this file to define LAYOUT_LR()

#include "yuanw.h"
#include "keycodes.h"
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif

// Secrets support
#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#    include "secrets.h"
#else
static const char secret_0[] PROGMEM = "test1";
static const char secret_1[] PROGMEM = "test2";
static const char secret_2[] PROGMEM = "test3";
static const char secret_3[] PROGMEM = "test4";
static const char secret_4[] PROGMEM = "test5";
#endif

static const char *const secrets[] PROGMEM = {
    secret_0, secret_1, secret_2, secret_3, secret_4,
};

static bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SECRET_1 ... KC_SECRET_2:
            if (record->event.pressed) {
                clear_mods();
                clear_oneshot_mods();
                send_string_P((char *)pgm_read_ptr(&secrets[keycode - KC_SECRET_1]));
            }
            return false;
    }
    return true;
}

enum layers { BASE, SYM, FUN, NAV, WIN, PNT, NUM, TXT, REP };

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
    BSPC_FUN = LT(TXT, KC_BSPC),
    ENT_SYM  = LT(SYM, KC_ENT),
    R_NUM    = LT(NUM, KC_R),
    // https://getreuer.info/posts/keyboards/faqs/index.html#layer-tap-repeat-key
    REP_TXT = LT(TXT, KC_0),
    MAGIC   = QK_AREP,

    U_RDO = SCMD(KC_Z),
    U_PST = LCMD(KC_V),
    U_CPY = LCMD(KC_C),
    U_CUT = LCMD(KC_X),
    U_UND = LCMD(KC_Z),

    LR_DOT = LT(SYM, KC_DOT),
    LR_G   = LT(SYM, KC_G),
};

// Shared combos for QK_REP and QK_AREP access
const uint16_t PROGMEM combo_rep_thumb[]  = {KC_SCLN, LR_DOT, COMBO_END};
const uint16_t PROGMEM combo_rep_top[]    = {KC_M, LR_G, COMBO_END};
const uint16_t PROGMEM combo_arep_bot[]   = {KC_M, LR_G, COMBO_END};
const uint16_t PROGMEM combo_arep_thumb[] = {R_NUM, SPC_NAV, COMBO_END};
const uint16_t PROGMEM combo_bspc_thumb[] = {R_NUM, REP_TXT, COMBO_END};
combo_t                key_combos[]       = {
    COMBO(combo_rep_thumb, KC_B), COMBO(combo_rep_top, KC_W), COMBO(combo_arep_thumb, KC_BSPC), COMBO(combo_bspc_thumb, KC_BSPC),
};

// clang-format off

// Keymaps using LAYOUT_LR() abstraction
// LAYOUT_LR is designed for 3x5 split layout:
//   - 5 columns left, 5 columns right for rows 0-2 (3 rows)
//   - 3 left thumb keys, 2 right thumb keys
//
// Layout diagram:
//   L00 L01 L02 L03 L04    R00 R01 R02 R03 R04   <- Row 0
//   L10 L11 L12 L13 L14    R10 R11 R12 R13 R14   <- Row 1
//   L20 L21 L22 L23 L24    R20 R21 R22 R23 R24   <- Row 2
//             LT0 LT1 LT2       RT0 RT1          <- Thumbs (3 left, 2 right)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT_LR(
        KC_Q,    KC_Y,    KC_O,   KC_U,    KC_EQL,                         KC_X,    KC_L,    KC_D,    KC_P,    KC_Z,
        HRM_C,   HRM_I,   HRM_A,  HRM_E,   KC_MINS,                        KC_K,    HRM_H,   HRM_T,   HRM_N,   HRM_S,
         KC_QUOT, KC_COMM, LR_DOT, KC_SCLN, KC_SLASH,                      KC_J,    KC_M,    LR_G,    KC_F,    KC_V,
                                   ESC_WIN, SPC_NAV, ARCANE,               R_NUM, REP_TXT
    ),

    [SYM] = LAYOUT_LR(
         KC_GRV,  KC_AMPR, KC_PERC, KC_PAST, XXXXXXX,                       XXXXXXX, KC_LCBR, KC_RCBR, KC_DLR,  ARROW,
        KC_EXLM, KC_LABK, KC_RABK, EMAIL_1, XXXXXXX,                       XXXXXXX, KC_LPRN, KC_RPRN, KC_AT,   KC_CIRC,
        KC_TILD, KC_BSLS, KC_SCLN, KC_PIPE, KC_BSLS,                       XXXXXXX, KC_LBRC, KC_RBRC, KC_HASH, XXXXXXX,
                                   XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX
    ),

    [FUN] = LAYOUT_LR(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX
    ),

    [NAV] = LAYOUT_LR(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_PGUP, KC_HOME, KC_UP,   KC_END,  XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_PGDN, KC_LEFT, KC_DOWN, KC_RIGHT,XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       RDO,     PST,     CPY,     CUT,     UND,
                                   XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, QK_LLCK
     ),

    [WIN] = LAYOUT_LR(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, LAG(KC_1), LAG(KC_2), LAG(KC_3), LAG(KC_4),
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, LSG(KC_1), LSG(KC_2), LSG(KC_3), LSG(KC_4),
                                   XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX
    ),

    [PNT] = LAYOUT_LR(
        _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______,
        _______, _______, QK_MOUSE_WHEEL_UP, QK_MOUSE_WHEEL_DOWN, _______,                       _______, _______, _______, _______, _______,
                                   QK_LLCK, KC_BTN1, KC_BTN2,              XXXXXXX, XXXXXXX
    ),

    [NUM] = LAYOUT_LR(
        QK_BOOT, KC_7,    KC_8,    KC_9,    KC_PAST,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_4,    KC_5,    KC_6,    XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_1,    KC_2,    KC_3,    XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   XXXXXXX, KC_0,    QK_LLCK,              XXXXXXX, XXXXXXX
    ),

    [TXT] = LAYOUT_LR(
        QK_BOOT,    KC_8,    KC_9, KC_SECRET_1, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        SELWORD, SELWBAK, SELLINE, KC_SECRET_2, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_1,    KC_2,    KC_3,    XXXXXXX,     XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   QK_LLCK, KC_MINS, XXXXXXX,              XXXXXXX, XXXXXXX
    ),

    [REP] = LAYOUT_LR(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, QK_AREP, KC_LSFT, XXXXXXX,                       XXXXXXX, KC_LSFT, QK_AREP, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   XXXXXXX, QK_REP,  XXXXXXX,              XXXXXXX, QK_REP
    ),
};
// clang-format on

const custom_shift_key_t custom_shift_keys[] = {
    {SPC_NAV, KC_TAB},  // Shift SPC is tab.
    {ESC_WIN, KC_ENT},  // Shift esc is enter.
    {BSPC_FUN, KC_DEL}, // Shift BSPC is DEL
};

uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

static bool g_last_key_on_right = false;
static bool g_arcane_is_repeat  = false;

bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    switch (keycode) {
        case ARCANE:
        case REP_TXT:
            return false; // Don't let arcane keys overwrite last-key state.
    }
    g_last_key_on_right = record->event.key.row >= MATRIX_ROWS / 2;
    return true;
}

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
#define MAGIC_STRING(str, repeat_keycode) magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char *str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT_LSHIFT);
    }

    send_string_P(str); // Send the string.
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
//     A * -> AO     L * -> LK      S * -> SON
//     C * -> CO     M * -> MENT    T * >  TION
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PH     Y * -> YP
//     G * -> GY     Q * -> QUEN    w * -> h
//     I * -> ION    R * -> RL      spc * -> the
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
    keycode = get_tap_keycode(keycode);

    if (mods == MOD_BIT_LALT) {
        switch (keycode) {
            case KC_U:
                return A(KC_O);
            case KC_O:
                return A(KC_U);
            case KC_N:
                return A(KC_I);
            case KC_I:
                return A(KC_N);
        }
    } else if ((mods & ~MOD_MASK_SHIFT) == 0) {
        // This is where most of the "magic" for the MAGIC key is implemented.
        switch (keycode) {
            case KC_SPC: // spc -> THE
            case KC_ENT:
            case KC_TAB:
            case SPC_NAV:
                return M_THE;

                // n -> '
            case KC_N:
                return KC_QUOT;

            // Fix SFBs and awkward strokes.
            case KC_A:
                return KC_O; // A -> O
            case KC_O:
                return KC_A; // O -> A
            case KC_E:
                return KC_U; // E -> U
            case KC_U:
                return KC_E; // U -> E
            case KC_I:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_ION; // I -> ON
                } else {
                    return KC_QUOT; // Shift I -> '
                }
            case KC_M:
                return M_MENT; // M -> ENT
            case KC_Q:
                return M_QUEN; // Q -> UEN
            case KC_T:
                return T_ION; // T -> TION

            case KC_C:
                return KC_O; // C -> O
            case KC_D:
                return KC_G; // D -> G
            case KC_G:
                return KC_Y; // G -> Y
            case KC_P:
                return KC_H; // P -> H
            case KC_Y:
                return KC_P; // Y -> P

            case KC_L:
                return KC_K; // L -> K
            case KC_S:
                return M_SON; // S -> T

            case KC_R:
                return KC_L; // R -> L
        case KC_W:
            return KC_H; // W -> H
            case KC_DOT:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_UPDIR; // . -> ./
                }
                return M_NOOP;
            case KC_HASH:
                return M_INCLUDE; // # -> include
            case KC_AMPR:
                return M_NBSP; // & -> nbsp;
            case KC_EQL:
                return M_EQEQ; // = -> ==
            case KC_RBRC:
                return KC_SCLN; // ] -> ;
            case KC_AT:
                return SHIP_IT; // @ -> SHIP_IT

            case KC_COMM:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return KC_EQL; // ! -> =
                }
                return M_NOOP;
            case KC_QUOT:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return M_DOCSTR; // " -> ""<cursor>"""
                }
                return M_NOOP;
            case KC_GRV: // ` -> ``<cursor>``` (for Markdown code)
                return M_MKGRVS;
            case KC_LABK: // < -> - (for Haskell)
                return KC_MINS;
            case KC_SLSH:
                return KC_SLSH; // / -> / (easier reach than Repeat)

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
        case KC_WH_U:
            return KC_WH_D;
        case KC_WH_D:
            return KC_WH_U;
        case SELWBAK:
            return SELWORD;
        case SELWORD:
            return SELWBAK;
    }
    return KC_TRNS;
}

static void handle_arcane(keyrecord_t *record) {
    if (record->event.pressed) {
        bool arcane_on_right  = (record->event.key.row >= MATRIX_ROWS / 2);
        g_arcane_is_repeat = (arcane_on_right == g_last_key_on_right);
    }
    if (g_arcane_is_repeat) {
        repeat_key_invoke(&record->event);     // same hand → repeat
    } else {
        alt_repeat_key_invoke(&record->event); // opposite hand → magic
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
    // If alt repeating key A, E, I, O, U, Y with no mods other than Shift, set
    // the last key to KC_N. Above, alternate repeat of KC_N is defined to be
    // again KC_N. This way, either tapping alt repeat and then repeat (or
    // equivalently double tapping alt repeat) is useful to type certain patterns
    // without SFBs:
    //
    //   D <altrep> <rep> -> DYN (as in "dynamic")
    //   O <altrep> <rep> -> OAN (as in "loan")
    if (get_repeat_key_count() < 0 && (all_mods & ~MOD_MASK_SHIFT) == 0 && (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }
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
        case ARCANE:
            handle_arcane(record);
            return false;

        case REP_TXT:
            if (record->tap.count) {
                handle_arcane(record); // was: alt_repeat_key_invoke
                return false;
            }
            break;

        case CPY:
            if (record->event.pressed) {
                switch (detected_host_os()) {
                    case OS_MACOS:
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
                    case OS_MACOS:
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
                    case OS_MACOS:
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
                    case OS_MACOS:
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
                    case OS_MACOS:
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
    if (record->event.pressed) {
        switch (keycode) {
            case SHIP_IT:
                MAGIC_STRING("Workiva/release-management-p", KC_AT);
                return false;
            case EMAIL_1:
                SEND_STRING_DELAY("yuan.wang@workiva.com", TAP_CODE_DELAY);
                return false;
            case EMAIL_2:
                SEND_STRING_DELAY("me@yuanwang.ca", TAP_CODE_DELAY);
                return false;
            case UPDIR:
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
                return false;

            case M_THE:
                MAGIC_STRING(/* */ "the", KC_N);
                break;
            case M_ION:
                MAGIC_STRING(/*i*/ "on", KC_S);
                break;
            case M_MENT:
                MAGIC_STRING(/*m*/ "ent", KC_S);
                break;
            case M_QUEN:
                MAGIC_STRING(/*q*/ "ue", KC_N);
                break;
            case T_ION:
                MAGIC_STRING(/*t*/ "io", KC_S);
                break;
            case M_SON:
                MAGIC_STRING(/*s*/ "on", KC_S);
                break;
            case M_UPDIR:
                MAGIC_STRING(/*.*/ "./", UPDIR);
                break;
        }
    }

    return true; // Process all other keycodes normally
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_keymap(keycode, record) && process_record_secrets(keycode, record);
}
