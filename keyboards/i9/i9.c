// Copyright 2022 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "i9.h"

encoder_behavior_t encoder_behavior = {KC_NO, KC_NO, KC_NO};

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

bool handle_encoder_user(encoder_action_t action) {
    return true;
}

void set_encoder_behavior(uint16_t encoder_press, uint16_t encoder_cw, uint16_t encoder_ccw) {
    encoder_behavior.press = encoder_press;
    encoder_behavior.cw = encoder_cw;
    encoder_behavior.ccw = encoder_ccw;
}

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

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;

	// Since there is only one encoder, we dispense with the index verification. It keeps things more readable.
	if (clockwise) {
        handle_encoder(ENC_CW);
    } else {
        handle_encoder(ENC_CCW);
    }
    return false;
}
