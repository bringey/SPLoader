/*
** File: include/SPLoader/kbd/Key.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KEY_H
#define _SPLOADER_KBD_KEY_H



#ifndef __ASM__

#define keycode(row, col) ((row << 5) | col)

typedef enum Key_e {

    // Row 0
    KEY_ESCAPE      = keycode(0, 0),
    KEY_F1          = keycode(0, 2),
    KEY_F2          = keycode(0, 3),
    KEY_F3          = keycode(0, 4),
    KEY_F4          = keycode(0, 5),
    KEY_F5          = keycode(0, 6),
    KEY_F6          = keycode(0, 7),
    KEY_F7          = keycode(0, 8),
    KEY_F8          = keycode(0, 9),
    KEY_F9          = keycode(0, 10),
    KEY_F10         = keycode(0, 11),
    KEY_F11         = keycode(0, 12),
    KEY_F12         = keycode(0, 13),
    KEY_PRINTSCRN   = keycode(0, 14),
    KEY_SCROLL_LOCK = keycode(0, 15),
    KEY_PAUSE       = keycode(0, 16),
    // Row 1
    KEY_TILDA       = keycode(1, 0),
    KEY_1           = keycode(1, 1),
    KEY_2           = keycode(1, 2),
    KEY_3           = keycode(1, 3),
    KEY_4           = keycode(1, 4),
    KEY_5           = keycode(1, 5),
    KEY_6           = keycode(1, 6),
    KEY_7           = keycode(1, 7),
    KEY_8           = keycode(1, 8),
    KEY_9           = keycode(1, 9),
    KEY_0           = keycode(1, 10),
    KEY_MINUS       = keycode(1, 11),
    KEY_EQUALS      = keycode(1, 12),
    KEY_BACKSPACE   = keycode(1, 13),
    KEY_INSERT      = keycode(1, 14),
    KEY_HOME        = keycode(1, 15),
    KEY_PAGE_UP     = keycode(1, 16),
    KEY_NUM_LOCK    = keycode(1, 17),
    KEY_NP_DIVIDE   = keycode(1, 18),
    KEY_NP_MULTIPLY = keycode(1, 19),
    KEY_NP_MINUS    = keycode(1, 20),
    // Row 2
    KEY_TAB         = keycode(2, 0),
    KEY_Q           = keycode(2, 1),
    KEY_W           = keycode(2, 2),
    KEY_E           = keycode(2, 3),
    KEY_R           = keycode(2, 4),
    KEY_T           = keycode(2, 5),
    KEY_Y           = keycode(2, 6),
    KEY_U           = keycode(2, 7),
    KEY_I           = keycode(2, 8),
    KEY_O           = keycode(2, 9),
    KEY_P           = keycode(2, 10),
    KEY_CLOSE_BRACE = keycode(2, 11),
    KEY_OPEN_BRACE  = keycode(2, 12),
    KEY_BACKSLASH   = keycode(2, 13),
    KEY_DELETE      = keycode(2, 14),
    KEY_END         = keycode(2, 15),
    KEY_PAGE_DOWN   = keycode(2, 16),
    KEY_NP_7        = keycode(2, 17),
    KEY_NP_8        = keycode(2, 18),
    KEY_NP_9        = keycode(2, 19),
    KEY_NP_PLUS     = keycode(2, 20),
    // Row 3
    KEY_CAPS_LOCK   = keycode(3, 0),
    KEY_A           = keycode(3, 1),
    KEY_S           = keycode(3, 2),
    KEY_D           = keycode(3, 3),
    KEY_F           = keycode(3, 4),
    KEY_G           = keycode(3, 5),
    KEY_H           = keycode(3, 6),
    KEY_J           = keycode(3, 7),
    KEY_K           = keycode(3, 8),
    KEY_L           = keycode(3, 9),
    KEY_SEMICOLON   = keycode(3, 10),
    KEY_QUOTE       = keycode(3, 11),
    KEY_ENTER       = keycode(3, 12),
    KEY_NP_4        = keycode(3, 17),
    KEY_NP_5        = keycode(3, 18),
    KEY_NP_6        = keycode(3, 19),
    // Row 4
    KEY_LSHIFT      = keycode(4, 0),
    KEY_Z           = keycode(4, 2),
    KEY_X           = keycode(4, 3),
    KEY_C           = keycode(4, 4),
    KEY_V           = keycode(4, 5),
    KEY_B           = keycode(4, 6),
    KEY_N           = keycode(4, 7),
    KEY_M           = keycode(4, 8),
    KEY_COMMA       = keycode(4, 9),
    KEY_PERIOD      = keycode(4, 10),
    KEY_SLASH       = keycode(4, 11),
    KEY_RSHIFT      = keycode(4, 12),
    KEY_UP          = keycode(4, 15),
    KEY_NP_1        = keycode(4, 17),
    KEY_NP_2        = keycode(4, 18),
    KEY_NP_3        = keycode(4, 19),
    KEY_NP_ENTER    = keycode(4, 20),
    // Row 5
    KEY_LCTRL       = keycode(5, 0),
    KEY_LSUPER      = keycode(5, 1),
    KEY_LALT        = keycode(5, 2),
    KEY_SPACE       = keycode(5, 3),
    KEY_RALT        = keycode(5, 10),
    KEY_RSUPER      = keycode(5, 11),
    KEY_MENU        = keycode(5, 12),
    KEY_RCTRL       = keycode(5, 13),
    KEY_LEFT        = keycode(5, 14),
    KEY_DOWN        = keycode(5, 15),
    KEY_RIGHT       = keycode(5, 16),
    KEY_NP_0        = keycode(5, 17),
    KEY_NP_PERIOD   = keycode(5, 19),
    KEY_RSVD = 0xFF

} Key;

#undef keycode

#endif  // __ASM__

#endif