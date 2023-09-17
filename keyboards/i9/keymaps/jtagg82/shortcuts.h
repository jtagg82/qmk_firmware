#pragma once

// these are custom short names to keep the layout cleaner to read
#define KH_ROT      LT(0, WHEEL)
#define KH_LCTL     LM(_60, MOD_LCTL)
#define KH_STAB     LSFT(KC_TAB)

#define WINTAB      LWIN(KC_TAB)
#define LASTWIN     LALT(KC_TAB)

#define KH_UNDO     LCTL(KC_Z)
#define KH_CUT      LCTL(KC_X)
#define KH_COPY     LCTL(KC_C)
#define KH_PAST     LCTL(KC_V)

#define CLOSE       LALT(KC_F4)
#define RELOAD      LCTL(KC_R)
#define BACK        LALT(KC_LEFT)
#define FORWARD     LALT(KC_RIGHT)

#define TASKMAN     C(S(KC_ESC))

#define NEWTAB      C(KC_T)
#define LASTTAB     C(S(KC_T))
#define CLOSTAB     C(KC_W)
