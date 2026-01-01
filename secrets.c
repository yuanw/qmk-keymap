#include QMK_KEYBOARD_H
#include "yuanw.h"

#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#include "secrets.h"
#else
const char secret_0[] PROGMEM = "test1";
const char secret_1[] PROGMEM = "test2";
const char secret_2[] PROGMEM = "test3";
const char secret_3[] PROGMEM = "test4";
const char secret_4[] PROGMEM = "test5";
#endif

static const char *const secrets[] PROGMEM = {
    secret_0, secret_1, secret_2, secret_3, secret_4,
};

bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
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
