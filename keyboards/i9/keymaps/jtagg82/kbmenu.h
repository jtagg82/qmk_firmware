// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H

#define KB_MENU_BACK_ITEM {"< Back", NULL, NULL, _kb_menu_back}

#define KB_MENU_MAX_NESTING 4

typedef struct kb_menu_entry_t kb_menu_item_t;

typedef struct {
    kb_menu_item_t *menu;
    uint8_t selection;
    char *name;
} kb_menu_nav_data_t;

// typedef struct {
//     char *name;
//     kb_menu_item_t *menu;
//     uint8_t menu_size;
// } kb_menu_t;

typedef struct {
    kb_menu_nav_data_t menu_stack[KB_MENU_MAX_NESTING];
    kb_menu_item_t *current_menu;
    uint8_t current_stack_depth;
    uint8_t current_selection;
    char *current_menu_name;
    char *current_selection_name;
    bool is_active :1;
    bool is_variable :1;
} kb_menu_state_t;



typedef bool (*kb_menu_fn_t)(kb_menu_state_t *state, void *user_data);

// typedef *kb_menu_entry_t kb_menu_t;
struct kb_menu_entry_t {
    char *name;
    // union {
        kb_menu_item_t *menu;
        // void *variable;
    // };
    void *options;
    kb_menu_fn_t on_select;
};


typedef struct {
    uint16_t min;
    uint16_t max;
    uint16_t delta;
} kb_menu_range_t;

// char *on_off[] = {"Off", "On"};
// uint8_t brightnesses[] = {31, 63, 95, 127, 159, 191, 223, 255};




typedef struct {
    uint8_t *variable;
    char **options;
} kb_menu_sel_var_opt_data_t;




kb_menu_state_t kb_menu_state;

void kb_menu_activate(kb_menu_item_t *menu);
void kb_menu_deactivate(void);
void kb_menu_next(void);
void kb_menu_prev(void);
void kb_menu_back(void);
void kb_menu_select(void);

bool _kb_menu_select(kb_menu_state_t *state, void *user_data);
bool _kb_menu_select_menu(kb_menu_state_t *state, void *user_data);
bool _kb_menu_select_var_with_options(kb_menu_state_t *state, void *user_data);
bool _kb_menu_select_var_with_range(kb_menu_state_t *state, void *user_data);
bool _kb_menu_select_value(kb_menu_state_t *state, void *user_data);

bool _kb_menu_next(kb_menu_state_t *state, void *user_data);
bool _kb_menu_prev(kb_menu_state_t *state, void *user_data);
bool _kb_menu_back(kb_menu_state_t *state, void *user_data);
