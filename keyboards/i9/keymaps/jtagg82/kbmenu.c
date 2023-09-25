// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "kbmenu.h"

void kbmenu_activate()
{
    kbmenu_is_active = true;
}

void kbmenu_deactivate()
{
    kbmenu_is_active = false;
}
