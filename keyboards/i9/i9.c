// Copyright 2022 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "i9.h"

/* Encoder behavior is simplified to a mapping of 3 keycodes for its basic functions.
   This makes it simple to set the desired keycodes to be activated when using the encoder.
   Use the set_encoder_behavior function to set the keycodes to be used.
*/
encoder_behavior_t encoder_behavior = {KC_NO, KC_NO, KC_NO};

void set_encoder_behavior(uint16_t encoder_press, uint16_t encoder_cw, uint16_t encoder_ccw) {
    encoder_behavior.press = encoder_press;
    encoder_behavior.cw = encoder_cw;
    encoder_behavior.ccw = encoder_ccw;
}

/* Simple encoder handler which taps the appropriate encoder_behavior keys. */
void handle_encoder(encoder_action_t action) {
    if (handle_encoder_user(action)) {
        switch(action) {
            case ENC_PRESS:
                tap_code16(encoder_behavior.press);
                break;
            case ENC_CW:
                tap_code16(encoder_behavior.cw);
                break;
            case ENC_CCW:
                tap_code16(encoder_behavior.ccw);
                break;
        }
    }
}

/* Weak, overridable function so that encoder behavior can be customized within a single function.
   This bypasses the use of the standard encoder behavior defined here and allows it to be more complex.
   Returns a true by default, which runs the default behavior. If false is returned in the override,
   the default behavior will simply be skipped.
*/
bool handle_encoder_user(encoder_action_t action) {
    return true;
}

/* Setting the standard encoder behavior*/
void keyboard_post_init_kb(void) {
    keyboard_post_init_user();
    set_encoder_behavior(KC_MUTE, KC_VOLU, KC_VOLD);
}

/* Override of the standard keyboard record processing.*/
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) return false;

    switch (keycode) {
		case ENC: // Handle encoder button press
            if (record->event.pressed) {
			    handle_encoder(ENC_PRESS);
                return false;
            }
			break;

		case KC_B: // Call bootloader
            if (record->event.pressed && (get_mods() == MOD_MASK_SHIFT)) // if B is pressed while both SHIFT keys are pressed
            {
				oled_invert(1);
                reset_keyboard(); // calls bootloader
                return false;
            }
            break;

        case KC_N: // Toggle NKRO
            if (record->event.pressed && (get_mods() == MOD_MASK_SHIFT)) // if N is pressed while both SHIFT keys are pressed
            {
				clear_keyboard(); // clear to prevent stuck keys
                keymap_config.nkro = !keymap_config.nkro;
                return false;
            }
            break;
    }
    return true;
}

/* Override of the encoder update function which is called when the knob is rotated.
   Since there is only one encoder, I ignore the index in this function.
   It simply calls a local handling function to aggregate actions happening with the encoder
   so that we don't have to spread encoder functionality across multiple locations.
*/
bool encoder_update_kb(uint8_t index, bool clockwise) {
    // Function can be overridden by the user to handle the encoder input
    if (!encoder_update_user(index, clockwise)) return false;

	if (clockwise) {
        handle_encoder(ENC_CW);
    } else {
        handle_encoder(ENC_CCW);
    }
    return false;
}

/* Implements a simple display which shows the states of the 3 locks. */
bool oled_task_kb(void) {
    if (!oled_task_user()) return false;

    led_t led_state = host_keyboard_led_state();

    oled_write_P(PSTR(" --  khaleput i9  --"), false);

    oled_advance_page(true);
    oled_advance_page(true);
	oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.caps_lock   ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.scroll_lock ? PSTR("  *  ")   : PSTR("     "), false);

    oled_advance_page(true);
	oled_write_P(PSTR(" Num   Caps   Scroll"), false);

    return false;
}
