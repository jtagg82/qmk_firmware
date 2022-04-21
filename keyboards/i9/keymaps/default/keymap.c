// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(	// default QWERTY layout
	 KC_ESC,      KC_F1 , KC_F2 , KC_F3 , KC_F4 ,      KC_F5 , KC_F6 , KC_F7 , KC_F8 ,       KC_F9 , KC_F10, KC_F11, KC_F12,   KC_PSCR,KC_SCRL,KC_PAUS,                           KC_MUTE,

	 KC_GRV,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,    KC_INS,KC_HOME,KC_PGUP,    KC_NUM,KC_PSLS,KC_PAST,KC_PMNS,
	   KC_TAB  ,  KC_Q ,  KC_W ,  KC_E ,  KC_R ,  KC_T ,  KC_Y ,  KC_U ,  KC_I ,  KC_O ,  KC_P ,KC_LBRC,KC_RBRC,  KC_BSLS  ,    KC_DEL, KC_END,KC_PGDN,    KC_P7 , KC_P8 , KC_P9 ,
	   KC_CAPS   ,  KC_A ,  KC_S ,  KC_D ,  KC_F ,  KC_G ,  KC_H ,  KC_J ,  KC_K ,  KC_L ,KC_SCLN,KC_QUOT,     KC_ENT      ,                               KC_P4 , KC_P5 , KC_P6 ,KC_PPLS,
	     KC_LSFT     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_N ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSFT       ,            KC_UP ,            KC_P1 , KC_P2 , KC_P3 ,
	 KC_LCTL , KC_LWIN , KC_LALT ,                     KC_SPACE                    , KC_RALT , KC_RWIN ,  KC_APP , KC_RCTL ,   KC_LEFT,KC_DOWN,KC_RGHT,        KC_P0     ,KC_PDOT,KC_PENT
	)
};


bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
    // We capture B and see if both SHIFTs are pressed. If so, we go to bootloader mode.
	case KC_B: ; // this semicolon is needed because we cannot have a declaration after a label
        const uint8_t mask = (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)); // mask which represents both SHIFT buttons
	    if (record->event.pressed && ((get_mods() & mask) == mask)) // if B is pressed as well as both SHIFT buttons
        {
	        reset_keyboard();
            return false;
	    }
	    return true;
	default:
	    return true;
    }
}

// Quadrature encoder behavior
bool encoder_update_user(uint8_t index, bool clockwise)
{
    if (index == 0)\
    { /* First encoder */
        if (clockwise)
		{
			tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;
}

#ifdef OLED_ENABLE
bool oled_task_user(void)
{
	oled_set_cursor(0, 0);
	oled_write_P(PSTR("      khaleput     "), false);

	// Host Keyboard Layer Status
	oled_set_cursor(0, 3);
	oled_write_P(PSTR(" Num   Caps   Scroll"), false);
	oled_set_cursor(0, 2);
    led_t led_state = host_keyboard_led_state();
	oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.caps_lock   ? PSTR("  *   ") : PSTR("      "), false);
	oled_write_P(led_state.scroll_lock ? PSTR("   *  ") : PSTR("      "), false);

    return false;
}
#endif



