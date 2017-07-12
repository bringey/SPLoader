/*
** File: include/SPLoader/ps2/keyboard.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_PS2_KEYBOARD_H
#define _SPLOADER_I386_PC_PS2_KEYBOARD_H

//
// PS/2 Keyboard Commands
//
#define PS2_CMD_SET_LED                        0xED
#define PS2_CMD_ECHO                           0xEE
#define PS2_CMD_GETSET_SCANCODE                0xF0
#define PS2_CMD_IDENTIFY                       0xF2
#define PS2_CMD_SET_TYPEMATIC                  0xF3
#define PS2_CMD_ENABLE_SCANNING                0xF4
#define PS2_CMD_DISABLE_SCANNING               0xF5
#define PS2_CMD_DEFAULTS                       0xF6
#define PS2_CMD_RESTRICT_TYPEMATIC             0xF7
#define PS2_CMD_RESTRICT_MAKE_RELEASE          0xF8
#define PS2_CMD_RESTRICT_MAKE                  0xF9
#define PS2_CMD_RESTRICT_ALL                   0xFA
#define PS2_CMD_RESTRICT_SPECIFIC_TYPEMATIC    0xFB
#define PS2_CMD_RESTRICT_SPECIFIC_MAKE_RELEASE 0xFC
#define PS2_CMD_RESTRICT_SPECIFIC_MAKE         0xFD
#define PS2_CMD_RESEND                         0xFE
#define PS2_CMD_RESET                          0xFF

//
// PS/2 Keyboard LED states
//
#define PS2_LED_SCROLL_LOCK 0x1
#define PS2_LED_NUMBER_LOCK 0x2
#define PS2_LED_CAPS_LOCK   0x4

//
// PS/2 Scancode sets
//
#define PS2_SCANCODE_QUERY 0 // get the current scancode set
#define PS2_SCANCODE_SET_1 1 // set keyboard scancode set to set 1
#define PS2_SCANCODE_SET_2 2 // ^ set 2
#define PS2_SCANCODE_SET_3 3 // ^ set 3


#endif