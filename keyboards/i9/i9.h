// Copyright 2022 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"


enum kbd_keycodes {
    ENC = QK_USER_MAX // Place the encoder keycode as the last possible one to not interfere with further definitions in keymaps
};

typedef enum {
    ENC_PRESS,
    ENC_CW,
    ENC_CCW
} encoder_action_t;

typedef struct {
    uint16_t press;
    uint16_t cw;
    uint16_t ccw;
} encoder_behavior_t;

extern encoder_behavior_t encoder_behavior;

void handle_encoder(encoder_action_t action);
__attribute__((weak)) bool handle_encoder_user(encoder_action_t action);
void set_encoder_behavior(uint16_t encoder_press, uint16_t encoder_cw, uint16_t encoder_ccw);

