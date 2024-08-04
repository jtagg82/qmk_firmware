// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>


// Defines names for use in layer keycodes and the keymap
enum layers {
    _QW,
	_QJ,
    _FN
};

const char *layer_names[] = {
	"QWERTY  ",
	"JTAGG   ",
	"Function"
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐            ┌───┐
     * │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Scr│Pse│            │Mut│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘            └───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │Ins│Hom│PgU│ │Num│ / │ * │ - │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │Del│End│PgD│ │ 7 │ 8 │ 9 │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │               │ 4 │ 5 │ 6 │   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │     │ ↑ │     │ 1 │ 2 │ 3 │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤Ent│
     * │Ctrl│GUI │Alt │                        │ Alt│ GUI│Menu│Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │   │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
     */
    [_QW] = LAYOUT(
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                               ENC,

        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                   KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT, KC_PENT,
                                                              TO(_QJ)
    ),
	/*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐            ┌───┐
     * │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Nxt│Ply│            │Mut│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘            └───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │Ins│Hom│PgU│ │Cal│ / │ * │ - │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │Del│End│PgD│ │ 7 │ 8 │ 9 │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
     * │ _FN  │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │               │ 4 │ 5 │ 6 │   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │     │ ↑ │     │ 1 │ 2 │ 3 │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤Ent│
     * │Ctrl│GUI │Alt │                        │ Alt│ _NV│LEAD│Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │   │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
     *                          \___FN__/
     */
	[_QJ] = LAYOUT(	// default QWERTY layout with a few tweaks
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_MNXT, KC_MPLY,                               ENC,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_CALC, KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,
        CW_TOGG, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                   KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPACE,                           KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT, KC_PENT,
                                                              TO(_QW)
	),
    /*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐            ┌───┐
     * │   │   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │   │Scr│Pse│            │Mut│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘            └───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
     * │Esc│F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│ Delete│ │   │   │   │ │Num│   │   │   │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
     * │     │   │   │   │   │   │   │PgU│ ↑ │PgD│   │   │   │Insrt│ │   │   │   │ │   │   │   │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤   │
     * │      │   │   │   │   │   │Hom│ ← │ ↓ │ → │End│   │        │               │   │   │   │   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
     * │        │   │   │   │   │   │   │   │   │   │   │          │     │   │     │   │   │   │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤   │
     * │    │    │    │                        │    │    │    │    │ │   │   │   │ │       │   │   │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
     */
    [_FN] = LAYOUT(
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, KC_SCRL, KC_PAUS,                               KC_MSEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,    KC_NUM , _______, _______, _______,
        _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______, _______, KC_PGUP, KC_UP,   KC_PGDN, _______, _______, _______, _______,    _______, _______, _______,    KC_BTN3, KC_MS_U, KC_WH_U,
        KC_CAPS, _______, _______, _______, _______, _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END,  _______,          _______,                                  KC_MS_L, KC_MS_D, KC_MS_R, _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             _______,             KC_WH_L, KC_WH_R, KC_WH_D,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______,    _______, _______, _______,    KC_BTN1,          KC_BTN2, _______,
                                                              _______
	)
	/* Copy the lines below to create a new transparent layer
	[___] = LAYOUT(
	    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,                               _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,    _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,    _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,                                  _______, _______, _______, _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             _______,             _______, _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______,    _______, _______, _______,    _______,          _______, _______,
                                                              _______
	)
    */
};

void keyboard_post_init_user(void) {
    set_encoder_behavior(KC_MUTE, KC_VOLU, KC_VOLD);
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
// 		   default:
//             break;
//     }
// 	return true;
// }

bool oled_task_user(void) {
	// Get current state of lock keys
	led_t led_state = host_keyboard_led_state();

// SIMPLE
/* 	oled_advance_page(true);
    oled_advance_page(true);
    oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
    oled_write_P(led_state.caps_lock   ? PSTR("  *    ") : PSTR("       "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("  *  ")   : PSTR("     "), false);
    oled_advance_page(true);
    oled_write_P(PSTR(" Num   Caps   Scroll"), false);*/

// NORMAL
 	oled_write_P(led_state.num_lock    ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(is_caps_word_on() ? PSTR("WORD ") : led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCROLL ") : PSTR("       "), false);
    oled_write_P(keymap_config.nkro ? PSTR("NKRO") : PSTR("6KRO"), false);

    // oled_advance_page(true);
    // oled_write_P(PSTR("Layout: "), false);
    // oled_write(kbd_layout_names[current_kbd_layout], false);

    oled_advance_page(true);
    oled_write_P(PSTR("Layer: "), false);
    oled_write(layer_names[get_highest_layer(layer_state)], false);

// DEBUG
   /*  oled_write_P(led_state.num_lock    ? PSTR("N") : PSTR("."), false);
    oled_write_P(is_caps_word_on() ? PSTR("W") : led_state.caps_lock ? PSTR("C") : PSTR("."), false);
    oled_write_P(led_state.scroll_lock ? PSTR("S") : PSTR("."), false);

    oled_advance_page(true);
    // char buffer [1];
    for (int layer = 0; layer < 10; layer++) {
        oled_write_P(layer_state_is(layer)?PSTR("*"):PSTR("."), false);
    }

    oled_advance_page(true);
    uint8_t mods = get_mods();
    oled_write_P(mods & MOD_BIT(KC_LCTL) ? PSTR("C") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_LSFT) ? PSTR("S") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_LWIN) ? PSTR("W") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_LALT) ? PSTR("A") : PSTR("."), false);
    oled_write_P(PSTR("-"), false);
    oled_write_P(mods & MOD_BIT(KC_RALT) ? PSTR("A") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_RWIN) ? PSTR("W") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_RSFT) ? PSTR("S") : PSTR("."), false);
    oled_write_P(mods & MOD_BIT(KC_RCTL) ? PSTR("C") : PSTR("."), false);
 */
    return false;
}
