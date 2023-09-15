// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

//#include "config_common.h"

/* USB Device descriptor parameter */
// #define VENDOR_ID    0xFEED
// #define PRODUCT_ID   0x0000
// #define DEVICE_VER   0x0001
// #define MANUFACTURER khaleput
// #define PRODUCT      i9

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 21

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on
 *                  diode)
 *
 */

// These aren't used because I have my own matrix scanning.
// #define MATRIX_ROW_PINS {}
// #define MATRIX_COL_PINS {}

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION ROW2COL

// ENCODER
#define ENCODERS_PAD_A { D2 }
#define ENCODERS_PAD_B { D3 }
#define ENCODER_RESOLUTION 4
#define ENCODER_DEFAULT_POS 0x3

// OLED
#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8
#define OLED_SCROLL_TIMEOUT 0
#define OLED_FADE_OUT

// Tapping
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

// In CAPS Word, Shift will uncapitalize
#define CAPS_WORD_INVERT_ON_SHIFT

// Leader key
// #define LEADER_TIMEOUT 300
// #define LEADER_NO_TIMEOUT

// Always send Escape if Control is pressed so that we get task manager
#define GRAVE_ESC_CTRL_OVERRIDE

// Unicode support type using wincompose
#define UNICODE_SELECTED_MODES UC_WINC

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
// #define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
// #define LOCKING_RESYNC_ENABLE

/*
 * Force NKRO
 *
 * Force NKRO (nKey Rollover) to be enabled by default, regardless of the saved
 * state in the bootmagic EEPROM settings. (Note that NKRO must be enabled in the
 * makefile for this to work.)
 *
 * If forced on, NKRO can be disabled via magic key (default = LShift+RShift+N)
 * until the next keyboard reset.
 *
 * NKRO may prevent your keystrokes from being detected in the BIOS, but it is
 * fully operational during normal computer usage.
 *
 * For a less heavy-handed approach, enable NKRO via magic key (LShift+RShift+N)
 * or via bootmagic (hold SPACE+N while plugging in the keyboard). Once set by
 * bootmagic, NKRO mode will always be enabled until it is toggled again during a
 * power-up.
 *
 */
#define FORCE_NKRO

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT


/* Bootmagic Lite key configuration */
//#define BOOTMAGIC_LITE_ROW 0
//#define BOOTMAGIC_LITE_COLUMN 0
