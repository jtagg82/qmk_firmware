// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

//#include "config_common.h"

// ENCODER
// #define ENCODERS_PAD_A { D2 }
// #define ENCODERS_PAD_B { D3 }
// #define ENCODER_RESOLUTION 4
#define ENCODER_DEFAULT_POS 0x3 // this one is not present in info.json

// OLED
#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8
#define OLED_SCROLL_TIMEOUT 0
#define OLED_FADE_OUT

// Tapping
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

// Always send Escape if Control is pressed so that we get task manager
#define GRAVE_ESC_CTRL_OVERRIDE

// Unicode support type using wincompose
#define UNICODE_SELECTED_MODES UC_WINC

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
