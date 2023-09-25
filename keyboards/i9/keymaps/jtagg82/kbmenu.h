// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H

#define SYSMENU {"System>", MENU, .menu = system_menu, 0, 0}
#define BACKMENU {"<Back", MENUBACK, 0, 0, 0}

#define MAX_NESTED_MENU 4

typedef enum {
    MENU,
    MENUBACK,
    FUNCTION,
    VARIABLE
} kbmenu_actions_t;

// typedef enum {
//     LIST,
//     GENERATOR
// } kbmenu_assign_mode_t;

// This will carry the current position within each submenu. It will need to be a struct to contain menu and location in menu.
uint8_t menu_stack[MAX_NESTED_MENU];
uint8_t current_menu_depth;


typedef struct kbmenu_entry_t kbmenu_entry_t;

struct kbmenu_entry_t {
    char *name;
    kbmenu_actions_t action;
    union {
        kbmenu_entry_t *menu;
        void *variable;
        void (*callback)(void *value);
    };
    void *options;
    uint8_t num_options;
};

bool brightness;

const char *on_off[] = {"Off", "On"};
const uint8_t brightnesses[] = {31, 63, 95, 127, 159, 191, 223, 255};

kbmenu_entry_t system_menu[3] = {
    {"Brightness", VARIABLE, .variable = (void*)&brightness, (void*)brightnesses, 7},
    SYSMENU,
    BACKMENU
};


kbmenu_entry_t sys = SYSMENU;




bool kbmenu_is_active;

kbmenu_entry_t *active_menu;

kbmenu_entry_t active_entry;


void kbmenu_next();

void kbmenu_prev();

void kbmenu_select();

void kbmenu_back();

void kbmenu_activate();

void kbmenu_deactivate();
