// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum encoder_functions {
    VOL,
    SCROLL,
    UNDO,
    UNDO_Y
};

enum encoder_functions encoder_state = VOL;

enum my_keycodes {
    ENCOCLICK = SAFE_RANGE
};

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KC_ESC,          KC_F1,
	KC_GRAVE, KC_1,  KC_2,
	KC_TAB,   KC_Q,  KC_W
    ),
    //[_FN] = LAYOUT(
    //    _______, _______,  _______, RESET, RESET    ,  XXXXXXX, _______
    //)
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise)
	{	
	    switch (encoder_state) {
                case VOL:
		    tap_code(KC_VOLU);
		    break;
		case SCROLL:
		    tap_code(KC_DOWN);
		    break;
		case UNDO:
		    tap_code16(C(S(KC_Z)));
		    break;
		case UNDO_Y:
		    tap_code16(C(KC_Y));
		    break;
		default:
		    break;
	    }
        } else {
            switch (encoder_state) {
                case VOL:
		    tap_code(KC_VOLD);
		    break;
		case SCROLL:
		    tap_code(KC_UP);
		    break;
		case UNDO:
		    tap_code16(C(KC_Z));
		    break;
		case UNDO_Y:
		    tap_code16(C(KC_Z));
		    break;
		default:
		    break;
	    };
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
	case ENCOCLICK:
	    if (record->event.pressed) {
	        switch (encoder_state) {
		    case VOL:
		        encoder_state = SCROLL;
		        break;
		    case SCROLL:
			encoder_state = UNDO;
			break;
		    case UNDO:
			encoder_state = UNDO_Y;
			break;
		    case UNDO_Y:
			encoder_state = VOL;
			break;
		    default:
			break;
		}
	    }
	    return false;
	default:
	    return true;
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer:    "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _FN:
            oled_write_P(PSTR("FN\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    oled_write_P(PSTR("Encoder:  "), false);

    switch (encoder_state) {
        case VOL:
            oled_write_P(PSTR("Volume\n"), false);
            break;
        case SCROLL:
            oled_write_P(PSTR("Scrolling\n"), false);
            break;
        case UNDO:
            oled_write_P(PSTR("Undo (SH)\n"), false);
            break;
        case UNDO_Y:
            oled_write_P(PSTR("Undo (Y)\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR(" "), false);
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCROLL") : PSTR("      "), false);

    return false;
}

#endif


