// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>

enum encoder_functions {
    VOL,
    SCROLL,
    UNDO,
    UNDO_Y
};

enum encoder_functions encoder_state = VOL;

enum my_keycodes {
    ENCO = SAFE_RANGE
};

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QW,
    _CM,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_QW] = LAYOUT(
	 KC_ESC,      KC_F1 , KC_F2 , KC_F3 , KC_F4 ,      KC_F5 , KC_F6 , KC_F7 , KC_F8 ,       KC_F9 , KC_F10, KC_F11, KC_F12,   KC_PSCR, ENCO  ,TG(_CM),                          KC_MUTE,
	 KC_GRV,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,    KC_INS,KC_HOME,KC_PGUP,   KC_CALC,KC_PSLS,KC_PAST,KC_PMNS,
	   KC_TAB  ,  KC_Q ,  KC_W ,  KC_E ,  KC_R ,  KC_T ,  KC_Y ,  KC_U ,  KC_I ,  KC_O ,  KC_P ,KC_LBRC,KC_RBRC,  KC_BSLS  ,    KC_DEL, KC_END,KC_PGDN,    KC_P7 , KC_P8 , KC_P9 ,
	   MO(_FN)   ,  KC_A ,  KC_S ,  KC_D ,  KC_F ,  KC_G ,  KC_H ,  KC_J ,  KC_K ,  KC_L ,KC_SCLN,KC_QUOT,     KC_ENT      ,                               KC_P4 , KC_P5 , KC_P6 , KC_PPLS,
	     KC_LSFT     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_N ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSFT       ,            KC_UP ,            KC_P1 , KC_P2 , KC_P3 ,
	 KC_LCTL , KC_LWIN , KC_LALT ,                     KC_SPACE                    , KC_RALT , KC_BTN1 , KC_BTN2 , KC_RCTL ,   KC_LEFT,KC_DOWN,KC_RGHT,        KC_P0     ,KC_PDOT, KC_PENT
    ),
    [_CM] = LAYOUT(
	 KC_ESC,      KC_F1 , KC_F2 , KC_F3 , KC_F4 ,      KC_F5 , KC_F6 , KC_F7 , KC_F8 ,       KC_F9 , KC_F10, KC_F11, KC_F12,   KC_PSCR, ENCO  ,_______,                           KC_MUTE,
	 KC_GRV,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,    KC_INS,KC_HOME,KC_PGUP,   KC_CALC,KC_PSLS,KC_PAST,KC_PMNS,
	   KC_TAB  ,  KC_Q ,  KC_W ,  KC_F ,  KC_P ,  KC_G ,  KC_J ,  KC_L ,  KC_U ,  KC_Y ,KC_SCLN,KC_LBRC,KC_RBRC,  KC_BSLS  ,    KC_DEL, KC_END,KC_PGDN,    KC_P7 , KC_P8 , KC_P9 ,
	   MO(_FN)   ,  KC_A ,  KC_R ,  KC_S ,  KC_T ,  KC_D ,  KC_H ,  KC_N ,  KC_E ,  KC_I ,  KC_O ,KC_QUOT,     KC_ENT      ,                               KC_P4 , KC_P5 , KC_P6 , KC_PPLS,
	     KC_LSFT     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_K ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSFT       ,            KC_UP ,            KC_P1 , KC_P2 , KC_P3 ,
	 _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   KC_LEFT,KC_DOWN,KC_RGHT,        KC_P0     ,KC_PDOT, KC_PENT
    ),
    [_FN] = LAYOUT(
        A(KC_F4),    _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,KC_SCRL,KC_PAUS,                           _______,
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______     ,   _______,_______,_______,    KC_NUM,_______,_______,_______,
          _______  ,KC_MPRV,KC_MPLY,KC_MNXT,_______,_______,_______,_______, KC_UP ,_______,_______,_______,_______,  _______  ,   _______,_______,_______,   _______,_______,_______,
           _______   ,_______,_______,_______,_______,_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,_______,_______,   _______       ,                              _______,_______,_______,_______,
             _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
         _______ , _______ , _______ ,                     _______                     , KC_CAPS , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
    ),

    //[_FN] = LAYOUT(
    //    _______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,_______,_______,                           _______,
    //    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______     ,   _______,_______,_______,   _______,_______,_______,_______,
    //      _______  ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,  _______  ,   _______,_______,_______,   _______,_______,_______,
    //       _______   ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______       ,                              _______,_______,_______,_______,
    //         _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
    //     _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
    //)
};

// Quadrature encoder behavior
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
	case ENCO:
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
        case _QW:
            oled_write_P(PSTR("QWERTY\n"), false);
            break;
        case _CM:
            oled_write_P(PSTR("COLEMAK\n"), false);
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
    oled_write_P(PSTR("   i9"), false);
    return false;
}
#endif


