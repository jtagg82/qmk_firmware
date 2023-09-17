// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <print.h>
#include "raw_hid.h"
#include "shortcuts.h"
#include "menu.h"


typedef union {
  uint32_t raw;
  struct {
	uint8_t   oled_brightness;
	uint8_t   default_layer;
	uint8_t   default_face;
    bool      nkro_status;
  };
} user_config_t;

user_config_t user_config;

enum my_keycodes {
    WHEEL = SAFE_RANGE
};

// Defines names for use in layer keycodes and the keymap
enum layers {
    _QW,
	_QJ,
    _FN,
    _6F,
    _60
};

const char *layer_names[] = {
	"_             ",
	"_             ",
	"Function      ",
    "-             ",
    "Shifted       "
};

// Defines the various display modes
typedef enum {
	DM_NORMAL,
	DM_MENU,
	DM_MAX // will always evaluate to the number of elements in the enum
} display_mode_t;

display_mode_t display_mode = DM_NORMAL;

// Defines the different wheel modes
typedef enum {
	WM_VOL,
	WM_MENU,
	WM_DISPLAY_BRIGHTNESS,
	WM_KBD_LAYOUT,
	WM_FACE,
	WM_NKRO,
	WM_MAX // will always evaluate to the number of elements in the enum
} wheel_mode_t;

wheel_mode_t wheel_mode = WM_VOL;

const char *wheel_mode_names[] = { // not currently using wheel mode names
	"Volume        ",
	"Menu          ",
	"Dsp. brghtness",
};

uint8_t display_brightness = 255;

typedef enum {
	MNU_DISP_BRIGHTNESS,
	MNU_KBD_LAYOUT,
	MNU_FACE,
	MNU_NKRO,
	MNU_BOOTLOADER,
	MNU_EXIT,
	MNU_MAX // will always evaluate to the number of elements in the enum
} menu_item_t;

const char *menu_item_names[] = {
	"Display brightness",
	"Keyboard layout   ",
	"Face              ",
	"NKRO              ",
	"Enter bootloader  ",
	"Exit menu         "
};

int8_t current_menu_item = 0;

typedef enum {
	KBL_QWERTY,
	KBL_QWERTYPLUS,
    KBL_60,
	KBL_MAX // will always evaluate to the number of elements in the enum
} kbd_layout_t;

const char *kbd_layout_names[] = {
	"QWERTY  ",
	"QWERTY+ ",
    "60%     "
};

int8_t current_kbd_layout = 1;

const char *nkro_names[] = {
	"Off",
	"On "
};

int8_t nkro_selection = 1;

typedef enum {
	FM_BASIC,
	FM_ADV,
    FM_DEBUG,
	FM_MAX // will always evaluate to the number of elements in the enum
} face_t;

const char *face_names[] = {
	"Basic     ",
	"Advanced  ",
    "Debug     "
};

int8_t current_face = 1;

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
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                               KH_ROT,

        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                   KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT, KC_PENT,
                                                              XXXXXXX
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
        KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_MNXT, KC_MPLY,                               KH_ROT,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_CALC, KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,
        CW_TOGG, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                   KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPACE,                           KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT, KC_PENT,
                                                              MO(_FN)
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
	),
     /*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐            ┌───┐
     * │   │   │   │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │   │Scr│Pse│            │Mut│
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
	[_6F] = LAYOUT(
        XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,                               KH_ROT,
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_BSPC,    XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_TAB,  _______, _______, _______, _______, _______, _______, KH_UNDO, TG(_60), _______, _______, _______, _______, KC_DEL,     XXXXXXX, XXXXXXX, XXXXXXX,    KC_BTN3, KC_MS_U, KC_WH_U,
        KH_STAB, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,          KC_ENT,                                   KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        XXXXXXX,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_UP,               XXXXXXX,             KC_WH_L, KC_WH_R, KC_WH_D,
        KH_LCTL, KC_LWIN, KC_LALT,                            KC_SPACE,                           KC_PSCR, KC_LEFT, KC_DOWN, KC_RGHT,    XXXXXXX, XXXXXXX, XXXXXXX,    KC_BTN1,          KC_BTN2, XXXXXXX,
                                                              TG(_60)
	),
    [_60] = LAYOUT(
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,                               _______,
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    _______, _______, _______,    _______, _______, _______, _______,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    _______, _______, _______,    _______, _______, _______,
        CW_TOGG, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                   _______, _______, _______, _______,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,             _______,             _______, _______, _______,
        KC_LCTL, KC_LWIN, KC_LALT,                            KC_SPACE,                           KC_RALT, KC_RGUI, KC_APP,  KC_RCTL,    _______, _______, _______,    _______,          _______, _______,
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

// custom tapping term per key
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
		case KH_ROT:
			return 1000;
        default:
            return TAPPING_TERM;
    }
}

void keyboard_post_init_user(void) {
	// Call the keymap level matrix init.
	// Read the user config from EEPROM
	user_config.raw = eeconfig_read_user();

	display_brightness = user_config.oled_brightness;
    oled_set_brightness(display_brightness);

	layer_move(user_config.default_layer);
    switch (user_config.default_layer) {
        default:
        case _QW:
            current_kbd_layout = 0;
            break;
        case _QJ:
            current_kbd_layout = 1;
            break;
        case _60:
            current_kbd_layout = 2;
            break;
    }

	keymap_config.nkro = user_config.nkro_status ? 1 : 0;
	current_face = user_config.default_face;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	const uint8_t mask = (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)); // mask representing both SHIFT keys
    switch (keycode) {
		case KH_ROT:
			if (record->tap.count && record->event.pressed) {
				switch (wheel_mode) {
					case WM_VOL:
						tap_code(KC_MUTE);
						break;
					case WM_MENU:
						switch (current_menu_item) {
							case MNU_DISP_BRIGHTNESS:
								wheel_mode = WM_DISPLAY_BRIGHTNESS;
								break;
							case MNU_KBD_LAYOUT:
								wheel_mode = WM_KBD_LAYOUT;
								break;
							case MNU_FACE:
								wheel_mode = WM_FACE;
								break;
							case MNU_NKRO:
								wheel_mode = WM_NKRO;
								break;
							case MNU_BOOTLOADER:
								oled_invert(1);
								reset_keyboard(); // calls bootloader
								break;
							case MNU_EXIT:
								display_mode = DM_NORMAL;
								wheel_mode = WM_VOL;
								oled_clear();
							default:
								break;
						}
						break;
					case WM_DISPLAY_BRIGHTNESS:
						wheel_mode = WM_MENU;
						user_config.oled_brightness = display_brightness;
						eeconfig_update_user(user_config.raw);
						break;
					case WM_KBD_LAYOUT:
						switch(current_kbd_layout) {
							case KBL_QWERTY:
								layer_move(_QW);
								break;
							case KBL_QWERTYPLUS:
								layer_move(_QJ);
								break;
                            case KBL_60:
								layer_move(_6F);
								break;
							default:
								break;
						}
						wheel_mode = WM_MENU;
						user_config.default_layer = get_highest_layer(layer_state);
						eeconfig_update_user(user_config.raw);
						break;
					case WM_FACE:
						wheel_mode = WM_MENU;
						user_config.default_face = current_face;
						eeconfig_update_user(user_config.raw);
						break;
					case WM_NKRO:
						clear_keyboard(); // clear to prevent stuck keys
						keymap_config.nkro = nkro_selection;
						user_config.nkro_status = nkro_selection == 1 ? true : false;
						eeconfig_update_user(user_config.raw);
						wheel_mode = WM_MENU;
						break;
					default:
						break;
				}
            } else if (record->event.pressed) {
				if (display_mode == DM_NORMAL) {
					display_mode = DM_MENU;
					wheel_mode = WM_MENU;
				} else {
					display_mode = DM_NORMAL;
					wheel_mode = WM_VOL;
				}
				oled_clear();
			}
			return false; // ignore normal behavior of button. There is none.
		case KC_B:
            if (record->event.pressed && ((get_mods() & mask) == mask)) // if B is pressed while both SHIFT keys are pressed
            {
				oled_invert(1);
                reset_keyboard(); // calls bootloader
                return false;
            }
            break;
    }
	return true;
}

// Wheel encoder behavior.
bool encoder_update_user(uint8_t index, bool clockwise) {
	// Since there is only one encoder, we dispense with the index verification. It keeps things more readable
	switch (wheel_mode) {
		case WM_VOL:
			if (IS_LAYER_ON(_FN)) {
				clockwise ? tap_code(KC_MFFD) : tap_code(KC_MRWD);
			} else {
				clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
			}
			break;
		case WM_MENU:
			if (clockwise) {
				current_menu_item++;
				if (current_menu_item >= MNU_MAX) current_menu_item -= MNU_MAX;
			} else {
				current_menu_item--;
				if (current_menu_item < 0) current_menu_item += MNU_MAX;
			}
			break;
		case WM_DISPLAY_BRIGHTNESS:
			display_brightness += (clockwise ? 16 : -16);
			oled_set_brightness(display_brightness);
			break;
		case WM_KBD_LAYOUT:
			if (clockwise) {
				current_kbd_layout++;
				if (current_kbd_layout >= KBL_MAX) current_kbd_layout -= KBL_MAX;
			} else {
				current_kbd_layout--;
				if (current_kbd_layout < 0) current_kbd_layout += KBL_MAX;
			}
			break;
		case WM_FACE:
			if (clockwise) {
				current_face++;
				if (current_face >= FM_MAX) current_face -= FM_MAX;
			} else {
				current_face--;
				if (current_face < 0) current_face += FM_MAX;
			}
			break;
		case WM_NKRO:
			if (clockwise) {
				nkro_selection++;
				if (nkro_selection >= 2) nkro_selection -= 2;
			} else {
				nkro_selection--;
				if (nkro_selection < 0) nkro_selection += 2;
			}
			break;
		default:
			break;
	}
    return false;
}

bool oled_task_user(void) {
	// Get current state of lock keys
	led_t led_state = host_keyboard_led_state();

	switch (display_mode) {
		case DM_NORMAL:
			switch (current_face) {
				case FM_BASIC:
					oled_advance_page(true);
					oled_advance_page(true);
					oled_write_P(led_state.num_lock    ? PSTR("  *    ") : PSTR("       "), false);
					oled_write_P(led_state.caps_lock   ? PSTR("  *    ") : PSTR("       "), false);
					oled_write_P(led_state.scroll_lock ? PSTR("  *  ")   : PSTR("     "), false);
					oled_advance_page(true);
					oled_write_P(PSTR(" Num   Caps   Scroll"), false);
					break;
				case FM_ADV:
					oled_write_P(led_state.num_lock    ? PSTR("N") : PSTR("."), false);
					oled_write_P(is_caps_word_on() ? PSTR("W") : led_state.caps_lock ? PSTR("C") : PSTR("."), false);
					oled_write_P(led_state.scroll_lock ? PSTR("S") : PSTR("."), false);
					for (int i = 0; i < 13; i++) {
                        oled_advance_char();
                    }
					oled_write_P(keymap_config.nkro ? PSTR("NKRO") : PSTR("6KRO"), false);

					oled_advance_page(true);
					oled_write_P(PSTR("Layout: "), false);
					oled_write(kbd_layout_names[current_kbd_layout], false);

					oled_advance_page(true);
					oled_write_P(PSTR("Layer : "), false);
					oled_write(layer_names[get_highest_layer(layer_state)], false);
					break;
                case FM_DEBUG:
                    oled_write_P(led_state.num_lock    ? PSTR("N") : PSTR("."), false);
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

                    break;

				default:
					break;
			}
			break;
		case DM_MENU:
			oled_write_P(led_state.num_lock    ? PSTR("N") : PSTR("."), true);
			oled_write_P(led_state.caps_lock   ? PSTR("C") : PSTR("."), true);
			oled_write_P(led_state.scroll_lock ? PSTR("S") : PSTR("."), true);
			oled_write_P(PSTR("      Menu        "), true);

			oled_advance_page(true);
			oled_write(menu_item_names[current_menu_item], false);

			oled_advance_page(true);
			switch (wheel_mode) {
				case WM_KBD_LAYOUT:
					oled_write(kbd_layout_names[current_kbd_layout], false);
					break;
				case WM_FACE:
					oled_write(face_names[current_face], false);
					break;
				case WM_DISPLAY_BRIGHTNESS:
					oled_write_P(PSTR("<"), false);
					for (int i = 0; i < 16; i++) {
						oled_write_P(PSTR(" "), i < display_brightness / 16);
					}
					oled_write_P(PSTR(">"), false);
					break;
				case WM_NKRO:
					oled_write(nkro_names[nkro_selection], false);
					break;
				default:
					oled_write_P(PSTR("                     "), false);
					break;
			}
			break;

		default:
			break;
    }
    return false;
}
