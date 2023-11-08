// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "kbmenu.h"



// kb_menu_entry_t kb_menu_main_menu = { "Menu", system_menu, NULL , _kb_menu_select_menu};

// External functions

void kb_menu_activate(kb_menu_item_t *menu) {
    kb_menu_state.is_active = true;
    kb_menu_state.current_stack_depth = 0; // don't need to empty the stack. The current depth points to the right place
    //kb_menu_state.menu_stack[0].menu = NULL;
    kb_menu_state.current_menu_name = "Menu";
    kb_menu_state.current_selection = 0;
    _kb_menu_select_menu(&kb_menu_state, (void *)menu); // select the menu that was passed as an argument
}

void kb_menu_deactivate(void) {
    kb_menu_state.is_active = false;
}

void kb_menu_next(void) {
    _kb_menu_next(&kb_menu_state, NULL);
}

void kb_menu_prev(void) {
    _kb_menu_prev(&kb_menu_state, NULL);
}

void kb_menu_back(void) {
    _kb_menu_back(&kb_menu_state, NULL);
}

void kb_menu_select(void) {
    _kb_menu_select(&kb_menu_state, NULL);
}

// Internal functions

bool _kb_menu_select(kb_menu_state_t *state, void *user_data){
    kb_menu_item_t *current_menu = &(state->current_menu[state->current_selection]);
    state->current_menu_name = current_menu->name; // only if I've just selected a menu
    current_menu->on_select(state, (void *)current_menu->menu);
    return true;
}

bool _kb_menu_select_menu(kb_menu_state_t *state, void *user_data) {
    kb_menu_item_t *selected_menu = (kb_menu_item_t *)user_data;

    // Push current menu info to stack
    state->menu_stack[state->current_stack_depth].menu = state->current_menu;
    state->menu_stack[state->current_stack_depth].selection = state->current_selection;
    state->menu_stack[state->current_stack_depth].name = state->current_menu_name;
    state->current_stack_depth++;

    // Replace current info
    state->current_menu = selected_menu;
    //state->current_menu_name = state->menu_stack[state->current_stack_depth - 1].name;
    state->current_selection = 0; // Set the current selection to 0 to start on the first.
    state->current_selection_name = selected_menu[state->current_selection].name;
    return true;
}

bool _kb_menu_select_var_with_options(kb_menu_state_t *state, void *user_data) {
    kb_menu_sel_var_opt_data_t *data = (kb_menu_sel_var_opt_data_t *)user_data;

    //data->options[0];

    // state contains current selection. It should be set to the current value of the variable first.
    // Assumption is that the selection is always numeric. Either a number, or an index from a list.
    state->current_selection = *(data->variable);
    return true;
}

bool _kb_menu_select_var_with_range(kb_menu_state_t *state, void *user_data) {

    return true;
}

// need to distinguish external commends from internal implementation
bool _kb_menu_next(kb_menu_state_t *state, void *user_data) {
    if (state->current_selection < 3) { // how to store this value?
        state->current_selection++;
        state->current_selection_name = state->current_menu[state->current_selection].name;
    }
    return true;
}

bool _kb_menu_prev(kb_menu_state_t *state, void *user_data) {
    if (state->current_selection > 0) {
        state->current_selection--;
        state->current_selection_name = state->current_menu[state->current_selection].name;
    }
    return true;
}

bool _kb_menu_back(kb_menu_state_t *state, void *user_data) {
    if (state->current_stack_depth > 0) {
        state->current_stack_depth--;
        state->current_menu = state->menu_stack[state->current_stack_depth].menu;
        state->current_selection = state->menu_stack[state->current_stack_depth].selection;
        state->current_menu_name = state->menu_stack[state->current_stack_depth - 1].name; // not a nice solution
        state->current_selection_name = state->current_menu[state->current_selection].name;
    }
    return true;
}

bool _kb_menu_select_value(kb_menu_state_t *state, void *user_data) {
    return true;
}




// for testing. ignore.




// uint8_t test_var;
// kb_menu_sel_var_opt_data_t test = { .variable = &test_var, on_off};
