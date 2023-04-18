// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>

enum my_keycodes { //unused
    JT_ENC = SAFE_RANGE,
    JT_SW,
    JT_SUDO,
    RD_LEFT,
    RD_RGHT
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
    _NV,
    _FN,
    _60,
    _6F
};

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

// these are custom short names to keep the layout below cleaner to read
#define JT_FN       LT(_FN, KC_CAPS)
#define JT_ESC      LT(0,KC_ESC) // unused
#define TD_NUM      TD(JT_NUM)
#define TD_CAP      TD(JT_CAPS)
#define TD_SCL      TD(JT_SCROLL)
#define TD_F4       TD(JT_F4)
#define WINTAB      LWIN(KC_TAB)
#define LASTWIN     LALT(KC_TAB)
#define UNDO        LCTL(KC_Z)
#define CUT         LCTL(KC_X)
#define COPY        LCTL(KC_C)
#define PASTE       LCTL(KC_V)
#define CLOSE       LALT(KC_F4)
#define RELOAD      LCTL(KC_R)
#define BACK        LALT(KC_LEFT)
#define FORWARD     LALT(KC_RIGHT)
#define TASKMAN     C(S(KC_ESC))
#define CAPTURE     G(A(KC_G))
#define NEWTAB      C(KC_T)
#define LASTTAB     C(S(KC_T))
#define CLOSTAB     C(KC_W)

qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for num lock, twice for Calculator
    [JT_NUM] = ACTION_TAP_DANCE_DOUBLE(KC_CALC, KC_NUM),
	[JT_CAPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
	[JT_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, scroll_finished, scroll_reset),
	[JT_F4] = ACTION_TAP_DANCE_DOUBLE(KC_F4, A(KC_F4))
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
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
     */
	[_QW] = LAYOUT(	// default QWERTY layout with a few tweaks
	KC_ESC ,      KC_F1 , KC_F2 , KC_F3 , KC_F4 ,      KC_F5 , KC_F6 , KC_F7 , KC_F8 ,       KC_F9 , KC_F10, KC_F11, KC_F12,   KC_PSCR,KC_MNXT,KC_MPLY,                         KC_MUTE,

	KC_GRV , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  , KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_MINS,KC_EQL ,       KC_BSPC ,   KC_INS ,KC_HOME,KC_PGUP,   KC_CALC,KC_PSLS,KC_PAST,KC_PMNS,
	KC_TAB     , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  , KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_LBRC,KC_RBRC,   KC_BSLS ,   KC_DEL ,KC_END ,KC_PGDN,    KC_P7 , KC_P8 , KC_P9 ,
	JT_FN        , KC_A  , KC_S  , KC_D  , KC_F  , KC_G  , KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,         KC_ENT  ,                               KC_P4 , KC_P5 , KC_P6 ,KC_PPLS,
    KC_LSFT          , KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  , KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,             KC_RSFT ,            KC_UP ,            KC_P1 , KC_P2 , KC_P3 ,
	KC_LCTL  , KC_LWIN , KC_LALT ,                     KC_SPACE                    , KC_RALT , TG(_NV) , KC_LEAD , KC_RCTL ,   KC_LEFT,KC_DOWN,KC_RGHT,        KC_P0     ,KC_PDOT,KC_PENT
	),
    /*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐            ┌───┐
     * │   │   │   │   │   │   │ │   │   │   │   │ │   │   │   │   │ │   │Scr│Pse│            │Mut│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘            └───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │       │ │   │   │   │ │NUM│   │   │   │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
     * │     │   │   │   │   │   │   │   │ ↑ │   │   │   │   │     │ │   │   │   │ │MB3│MSU│MWU│   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤   │
     * │ _FN  │   │   │   │   │   │   │ ← │ ↓ │ → │   │   │        │               │MSL│MSD│MSR│   │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
     * │  CAPS  │   │   │   │   │   │   │   │   │   │   │          │     │   │     │MWL│MWR│MWD│   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤   │
     * │    │    │    │                        │    │    │    │    │ │   │   │   │ │  MB1  │MB2│   │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
     */
	[_FN] = LAYOUT(
	_______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,KC_SCRL,KC_PAUS,                         KC_MSEL,

	_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______ ,   _______,_______,_______,   KC_NUM ,_______,_______,_______,
	_______    ,KC_MPRV,KC_MPLY,KC_MNXT,_______,_______,_______,_______, KC_UP ,_______,_______,_______,_______,   _______ ,   _______,_______,_______,   KC_BTN3,KC_MS_U,KC_WH_U,
	_______      ,_______,JT_SUDO,_______,_______,_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,_______,_______,         _______ ,                              KC_MS_L,KC_MS_D,KC_MS_R,_______,
	KC_CAPS          , UNDO  ,  CUT  , COPY  , PASTE ,_______,_______,_______,_______,_______,_______,             _______ ,           _______,           KC_WH_L,KC_WH_R,KC_WH_D,
	_______  , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   RD_LEFT,_______,RD_RGHT,       KC_BTN1    ,KC_BTN2,_______
	),
    /* This layer only modifies the function row
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
     * │   │   │NTb│   │LTb│CTb│ │Rld│   │LWn│CWn│ │Cap│   │   │TkM│ │Prv│Nxt│Ply│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
     */
    [_NV] = LAYOUT(
	_______,     NEWTAB ,LASTTAB,RELOAD ,CLOSTAB,     _______,_______,LASTWIN, CLOSE ,      CAPTURE,_______,_______,TASKMAN,   _______,_______,_______,                         _______,

	_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______     ,   _______,_______,_______,   _______,_______,_______,_______,
	_______    ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,  _______  ,   _______,_______,_______,   _______,_______,_______,
	_______      ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,   _______       ,                              _______,_______,_______,_______,
	_______          ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
	_______  , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
	),
	// [_60] = LAYOUT(
	// XXXXXXX,     XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,     XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,      XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,_______,XXXXXXX,                         _______,
	// KC_GESC,  KC_1 ,  KC_2 ,  KC_3 ,  KC_4 ,  KC_5 ,  KC_6 ,  KC_7 ,  KC_8 ,  KC_9 ,  KC_0 ,KC_MINS, KC_EQL,    KC_BSPC    ,   XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
	//    KC_TAB  ,  KC_Q ,  KC_W ,  KC_E ,  KC_R ,  KC_T ,  KC_Y ,  KC_U ,  KC_I ,  KC_O ,  KC_P ,KC_LBRC,KC_RBRC,  KC_DEL   ,   XXXXXXX,XXXXXXX,XXXXXXX,   XXXXXXX,XXXXXXX,XXXXXXX,
	//    KC_LCTL   ,  KC_A ,  KC_S ,  KC_D ,  KC_F ,  KC_G ,  KC_H ,  KC_J ,  KC_K ,  KC_L ,KC_SCLN,KC_QUOT,     KC_ENT      ,                              XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,
	// 	    KC_LSFT     ,  KC_Z ,  KC_X ,  KC_C ,  KC_V ,  KC_B ,  KC_N ,  KC_M ,KC_COMM, KC_DOT,KC_SLSH,       KC_RSFT       ,           XXXXXXX,           XXXXXXX,XXXXXXX,XXXXXXX,
	//  KC_LCTL , KC_LWIN , KC_LALT ,                  LT(_6F, KC_SPC)                , KC_RALT , KC_RWIN , KC_RSFT , KC_RCTL ,   XXXXXXX,XXXXXXX,XXXXXXX,       XXXXXXX    ,XXXXXXX,XXXXXXX
	// ),
	// [_6F] = LAYOUT(
	// _______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,_______,_______,                         _______,
	// _______, KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 , KC_F7 , KC_F8 , KC_F9 , KC_F10, KC_F11, KC_F12,   _______     ,   _______,_______,_______,   _______,_______,_______,_______,
	//   _______  ,_______,_______,_______,_______,_______,_______,KC_HOME, KC_UP , KC_END,KC_PGUP,_______, KC_INS,  KC_BSLS  ,   _______,_______,_______,   _______,_______,_______,
	//    _______   ,_______,_______,_______,_______,_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGDN,_______,   _______       ,                              _______,_______,_______,_______,
	// 	    _______     ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,       _______       ,           _______,           _______,_______,_______,
	//  _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______ ,   _______,_______,_______,       _______    ,_______,_______
	// )
	/* Copy these lines below to create a new layer
	[_FN] = LAYOUT(
	_______,     _______,_______,_______,_______,     _______,_______,_______,_______,      _______,_______,_______,_______,   _______,_______,_______,                         _______,
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
		case TD(JT_SCROLL):
			return 300;
        case JT_FN:
            return 150;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case JT_SW:
            if (record->event.pressed) {
                if (IS_LAYER_ON(_QW)) {
                    layer_move(_60);
                } else
                if (IS_LAYER_ON(_60)) {
                    layer_move(_QW);
                }
            }
            return false;
        case JT_SUDO:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_HOME)"sudo "SS_TAP(X_END));
            }
            return false;
        case RD_LEFT:
            if (record->event.pressed) {
                // Ctrl+Alt+Home followed by Ctrl+Win+Left
                SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_HOME)))SS_DELAY(500)SS_LCTL(SS_LWIN(SS_TAP(X_LEFT))));
            }
            return false;
        case RD_RGHT:
            if (record->event.pressed) {
                // Ctrl+Alt+Home followed by Ctrl+Win+Right
                SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_HOME)))SS_DELAY(500)SS_LCTL(SS_LWIN(SS_TAP(X_RIGHT))));
            }
            return false;
        default:
            return true;
    }
}

bool did_leader_succeed;
LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        did_leader_succeed = leading = false;
        SEQ_ONE_KEY(KC_MPLY) {
            tap_code(KC_PAUSE);
            did_leader_succeed = true;
        } else
        SEQ_ONE_KEY(KC_MNXT) {
            tap_code(KC_SCRL);
            did_leader_succeed = true;
        } else
        SEQ_TWO_KEYS(KC_P, KC_S) {
            tap_code(KC_SYSTEM_SLEEP);
            did_leader_succeed = true;
        } else
        SEQ_TWO_KEYS(KC_P, KC_O) {
            tap_code(KC_SYSTEM_POWER);
            did_leader_succeed = true;
        } else
        SEQ_TWO_KEYS(KC_B, KC_B) {
            oled_invert(1);
            reset_keyboard();
            did_leader_succeed = true;
        }/* else
        SEQ_TWO_KEYS(KC_E, KC_D) {
            SEND_STRING(SS_LGUI("r") "cmd\n" SS_LCTL("c"));
            did_leader_succeed = true;
        }*/
        leader_end();
    }
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
        }
    }
}

void leader_start(void) {

}

void leader_end(void) {
  if (did_leader_succeed) {

  } else {

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

// Quadrature encoder behavior
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (IS_LAYER_ON(_FN)) {
            if (clockwise)
            {
                tap_code(KC_MFFD);
            } else {
                tap_code(KC_MRWD);
            }
        // } else if (IS_LAYER_ON(_NV)) {
        //     if (clockwise)
        //     {
        //         if (!is_alt_tab_active) {
        //             is_alt_tab_active = true;
        //             register_code(KC_LALT);
        //         }
        //         alt_tab_timer = timer_read();
        //         tap_code16(KC_TAB);
        //     } else {
        //         if (!is_alt_tab_active) {
        //             is_alt_tab_active = true;
        //             register_code(KC_LALT);
        //         }
        //         alt_tab_timer = timer_read();
        //         tap_code16(S(KC_TAB));
        //     }
        } else {
            if (clockwise)
            {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
    return false;
}

bool oled_task_user(void) {
	oled_set_cursor(0, 0);
	//oled_write_P(PSTR("Layer:    "), false);

    switch (get_highest_layer(layer_state)) {
        case _QW:
            oled_write_P(PSTR("QWERTY "), false);
            break;
        case _FN:
            oled_write_P(PSTR("FUNC   "), false);
            break;
        case _NV:
            oled_write_P(PSTR("QWERTY+NAV"), false);
            break;
		case _60:
            oled_write_P(PSTR("60%    "), false);
            break;
		case _6F:
            oled_write_P(PSTR("60% Fn "), false);
            break;
        default:
            oled_write_P(PSTR("-Undef-"), false);
    }
    oled_write_P(leading?PSTR("  LEAD"):PSTR("      "), false);

    // oled_set_cursor(6,2);
    // oled_write_P(PSTR("0123456789 "), false);
    // char buffer[1];
    // oled_write(itoa(default_layer_state, buffer, 10), false);
    // oled_set_cursor(6,3);
    // for (int i = 0; i < 10; i++)
    // {
    //     oled_write_P(layer_state_is(i)?PSTR("^"):PSTR(" "), false);
    // }

	// Host Keyboard Layer Status
    led_t led_state = host_keyboard_led_state();
	oled_set_cursor(0, 3);
	oled_write_P(PSTR(" Num   Caps   Scroll"), false);
	oled_set_cursor(0, 2);
	oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.caps_lock   ? PSTR("  *    ") : PSTR("       "), false);
	oled_write_P(led_state.scroll_lock ? PSTR("  *  ")   : PSTR("     "),   false);



    // oled_write_P(led_state.num_lock    ? PSTR("N") : PSTR("."), false);
    // oled_write_P(led_state.caps_lock   ? PSTR("C") : PSTR("."), false);
    // oled_write_P(led_state.scroll_lock ? PSTR("S") : PSTR("."), false);


    /*
    // oled_set_cursor(0, 3);
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCROLL") : PSTR("      "), false);
	*/

	/*   This part writes the state of modifiers on the screen. Useless but fun.
	oled_set_cursor(9, 3);
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
