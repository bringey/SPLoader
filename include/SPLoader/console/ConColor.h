/*
** File: include/SPLoader/console/ConColor.h
**
** Author: Brennan Ringey
**
** Enum definition for all possible color values in the console. The console
** has a palette of 16 colors for both the foreground and background for each
** character. Color support depends on the driver.
*/

#ifndef _SPLOADER_CONSOLE_CONCOLOR_H
#define _SPLOADER_CONSOLE_CONCOLOR_H

#ifndef __ASM__

//
// ConColor enum. Based off of the default color palette for VGA 
// text-compatible mode.
//
typedef enum ConColor_e {
    CON_COLOR_BLACK          = 0x0,
    CON_COLOR_BLUE           = 0x1,
    CON_COLOR_GREEN          = 0x2,
    CON_COLOR_CYAN           = 0x3,
    CON_COLOR_RED            = 0x4,
    CON_COLOR_MAGENTA        = 0x5,
    CON_COLOR_BROWN          = 0x6,
    CON_COLOR_GRAY           = 0x7,
    CON_COLOR_DARK_GRAY      = 0x8,
    CON_COLOR_BRIGHT_BLUE    = 0x9,
    CON_COLOR_BRIGHT_GREEN   = 0xA,
    CON_COLOR_BRIGHT_CYAN    = 0xB,
    CON_COLOR_BRIGHT_RED     = 0xC,
    CON_COLOR_BRIGHT_MAGENTA = 0xD,
    CON_COLOR_YELLOW         = 0xE,
    CON_COLOR_WHITE          = 0xF,
} ConColor;

#endif  // __ASM__

#endif