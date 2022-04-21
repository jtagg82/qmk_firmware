// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>

enum my_keycodes { //unused
    JT_ENC = SAFE_RANGE,
};

// tap dance enum
enum tap_dance {
	JT_NUM = 0,
	JT_CAPS,
	JT_SCROLL,
	JT_F4
};

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Functions associated with individual tap dances
// CAPS LOCK key
void caps_finished(qk_tap_dance_state_t *state, void *user_data);
void caps_reset(qk_tap_dance_state_t *state, void *user_data);
// SCROLL LOCK key
void scroll_finished(qk_tap_dance_state_t *state, void *user_data);
void scroll_reset(qk_tap_dance_state_t *state, void *user_data);

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QW,
    _CM,
    _FN,
    _60,
    _6F
};

// these are custom short names to keep the layout below cleaner to read
#define JT_ESC LT(0,KC_ESC) // unused
#define TD_NUM TD(JT_NUM)
#define TD_CAP TD(JT_CAPS)
#define TD_SCL TD(JT_SCROLL)
#define TD_F4 TD(JT_F4)


qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for num lock, twice for Calculator
    [JT_NUM] = ACTION_TAP_DANCE_DOUBLE(KC_NUM, KC_CALC),
	[JT_CAPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
	[JT_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, scroll_finished, scroll_reset),
	[JT_F4] = ACTION_TAP_DANCE_DOUBLE(KC_F4, A(KC_F4))
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QW] = LAYOUT(	// default QWERTY layout with a few tweaks
	 KC_ESC,      KC_BF1 , KC_F2 , KC_F3 , TD_F4 ,      KC_F5 , KC_F6 , KC_F7 , KC_F8 ,       KC_F9 , KC_F10, KC_F11, KC_F12,   KC_PSCR, TD_SCL,KC_PAUS,                           KC_MUTE,
	 KC_GRV,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,    KC_INS,KC_HOME,KC_PGUP,    TD_NUM,KC_PSLS,KC_PAST,KC_PMNS,
	   KC_TAB  ,  KC_Q ,  KC_W ,  KC_E ,  KC_R ,  KC_T ,  KC_Y ,  KC_U ,  KC_I ,  KC_O ,  KC_P ,KC_LBRC,KC_RBRC,  KC_BSLS  ,    KC_DEL, KC_END,KC_PGDN,    KC_P7 , KC_P8 , KC_P9 ,
	   TD_CAP    ,  KC_A ,  KC_S ,  KC_D ,  KC_F ,  KC_G ,  KC_H ,  KC_J ,  KC_K ,  KC_L ,KC_SCLN,KC_QUOT,     KC_ENT      ,                               KC_P4 , KC_P5 , KC_P6 ,KC_PPLS,
	     KC_LSPO     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_N ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSPC       ,            KC_UP ,            KC_P1 , KC_P2 , KC_P3 ,
	 KC_LCTL , KC_LWIN , KC_LALT ,                     KC_SPACE                    , KC_RALT , KC_RWIN ,  KC_APP , KC_RCTL ,   KC_LEFT,KC_DOWN,KC_RGHT,        KC_P0     ,KC_PDOT,KC_PENT
	),
	[_CM] = LAYOUT( // Colemak layout (I don't use it so what's this for??)
	_______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,TO(_QW),_______,                           _______,
	_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,    _______    ,   _______,_______,_______,   _______,_______,_______,_______,
	  _______  ,  KC_Q ,  KC_W ,  KC_F ,  KC_P ,  KC_G ,  KC_J ,  KC_L ,  KC_U ,  KC_Y ,_______,_______,_______,  _______  ,   _______,_______,_______,   _______,_______,_______,
	   _______   ,  KC_A ,  KC_R ,  KC_S ,  KC_T ,  KC_D ,  KC_H ,  KC_N ,  KC_E ,  KC_I ,  KC_O ,_______,    _______      ,                              _______,_______,_______,_______,
	     _______     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_K ,  KC_M ,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
	 _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
	),
	[_FN] = LAYOUT(
	 _______,    _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,KC_SCRL,QK_BOOT,                           _______,
	_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______     ,   _______,_______,_______,    KC_NUM,_______,_______,_______,
	  _______  ,KC_MPRV,KC_MPLY,KC_MNXT,_______,_______,_______,_______, KC_UP ,_______,_______,_______,_______,  _______  ,   _______,_______,_______,   KC_BTN3,KC_MS_U,KC_WH_U,
	   _______   ,_______,_______,_______,_______,_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,_______,_______,   _______       ,                              KC_MS_L,KC_MS_D,KC_MS_R,_______,
		 _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           KC_WH_L,KC_WH_R,KC_WH_D,
	 _______ , _______ , _______ ,                     _______                     , KC_CAPS , _______ , _______ , _______ ,   _______,_______,_______,       KC_BTN1    ,KC_BTN2,_______
	),
	[_60] = LAYOUT(
	XXXXXXX,     XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,     XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,      XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,_______,XXXXXXX,                           KC_MUTE,
	KC_GESC,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,   XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
	   KC_TAB  ,  KC_Q ,  KC_W ,  KC_E ,  KC_R ,  KC_T ,  KC_Y ,  KC_U ,  KC_I ,  KC_O ,  KC_P ,KC_LBRC,KC_RBRC,  KC_DEL   ,   XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,XXXXXXX,XXXXXXX,
	   KC_LCTL   ,  KC_A ,  KC_S ,  KC_D ,  KC_F ,  KC_G ,  KC_H ,  KC_J ,  KC_K ,  KC_L ,KC_SCLN,KC_QUOT,     KC_ENT      ,                              XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
		 KC_LSFT     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_N ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSFT       ,           XXXXXXX,           XXXXXXX,XXXXXXX,XXXXXXX,
	 KC_LCTL , KC_LWIN , KC_LALT ,                  LT(_6F, KC_SPC)                , KC_RALT , KC_RWIN , KC_RSFT , KC_RCTL ,   XXXXXXX,XXXXXXX,XXXXXXX,       XXXXXXX    ,XXXXXXX,XXXXXXX
	),
	[_6F] = LAYOUT(
	_______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,_______,_______,                           _______,
	_______, KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 , KC_F7 , KC_F8 , KC_F9 , KC_F10, KC_F11, KC_F12,   _______     ,   _______,_______,_______,   _______,_______,_______,_______,
	  _______  ,_______,_______,_______,_______,_______,_______,KC_HOME, KC_UP , KC_END,KC_PGUP,_______, KC_INS,  KC_BSLS  ,   _______,_______,_______,   _______,_______,_______,
	   _______   ,_______,_______,_______,_______,_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGDN,_______,   _______       ,                              _______,_______,_______,_______,
		 _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
	 _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
	)
	/* Copy these lines below to create a new layer
	[_FN] = LAYOUT(
	_______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,_______,_______,                           _______,
	_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______     ,   _______,_______,_______,   _______,_______,_______,_______,
	  _______  ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,  _______  ,   _______,_______,_______,   _______,_______,_______,
	   _______   ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______       ,                              _______,_______,_______,_______,
		 _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
	 _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
	)
    */
};

// custom tapping term per key
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case JT_ESC: // unused
            return 500;
		case TD(JT_SCROLL):
			return 300;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
	case JT_ESC: // unused
	    if (!record->tap.count && record->event.pressed) {
	        tap_code16(A(KC_F4));
	        return false;
	    }
	    return true;
	default:
	    return true;
    }
}


// Initialize tap structure associated with example tap dance key
static td_tap_t caps_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};
static td_tap_t scroll_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void caps_finished(qk_tap_dance_state_t *state, void *user_data) {
    caps_tap_state.state = cur_dance(state);
    switch (caps_tap_state.state) {
        case TD_SINGLE_TAP:
            layer_invert(_FN);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_CAPS);
            break;
        default:
            break;
    }
}

void caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (caps_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_FN);
    }
    caps_tap_state.state = TD_NONE;
}

// Functions that control what our tap dance key does
void scroll_finished(qk_tap_dance_state_t *state, void *user_data) {
    scroll_tap_state.state = cur_dance(state);
    switch (scroll_tap_state.state) {
        case TD_SINGLE_TAP:
			switch (get_highest_layer(layer_state)) {
				case _QW:
					layer_move(_60);
					break;
				default:
					layer_move(_QW);
					break;
			}
            break;
        case TD_SINGLE_HOLD:
			layer_move(_QW);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_SCRL);
            break;
        default:
            break;
    }
}

void scroll_reset(qk_tap_dance_state_t *state, void *user_data) {
    scroll_tap_state.state = TD_NONE;
}


#ifdef ENCODER_ENABLE
// Quadrature encoder behavior
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise)
		{
			tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;
}
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
	oled_set_cursor(0, 0);
	//oled_write_P(PSTR("Layer:    "), false);

    switch (get_highest_layer(layer_state)) {
        case _QW:
            oled_write_P(PSTR("QWERTY "), false);
            break;
        case _CM:
            oled_write_P(PSTR("COLEMAK"), false);
            break;
        case _FN:
            oled_write_P(PSTR("Fn     "), false);
            break;
		case _60:
            oled_write_P(PSTR("60%    "), false);
            break;
		case _6F:
            oled_write_P(PSTR("60% Fn "), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_P(PSTR("-Undef-"), false);
    }

	// Host Keyboard Layer Status
	oled_set_cursor(0, 3);
	oled_write_P(PSTR(" Num   Caps   Scroll"), false);
	oled_set_cursor(0, 2);
    led_t led_state = host_keyboard_led_state();
	oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.caps_lock   ? PSTR("  *   ") : PSTR("      "), false);
	oled_write_P(led_state.scroll_lock ? PSTR("   *  ") : PSTR("      "), false);
	//oled_set_cursor(oled_max_chars() - 3, 2);

	/*   This part write the state of modifiers on the screen. Useless but fun.
	oled_write_P(PSTR("  "), false);
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


	/*
	for (int i = 0; i < 8; i++) {
		oled_write_P(mods % 2 ? PSTR("*") : PSTR("."), false);
		mods /= 2;
	} */

    //oled_write_P(PSTR("Encoder:  "), false);

/*     switch (encoder_state) {
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
            oled_write_ln(PSTR("Undefined"), false);
    } */
    /*
    oled_write_ln_P(PSTR(" "), false);
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCROLL") : PSTR("      "), false);
    oled_write_P(PSTR("   i9"), false);
	*/
    return false;
}
#endif



