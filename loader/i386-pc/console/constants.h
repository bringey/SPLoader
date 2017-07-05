/*
** File: loader/i386-pc/console/constants.h
**
** Author: Brennan Ringey
**
** Private header file containing macro constants for the i386-pc console
** driver.
**
*/

#ifndef _LOADER_I386_PC_CONSOLE_CONSTANTS_PH
#define _LOADER_I386_PC_CONSOLE_CONSTANTS_PH

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// no blink, black background, white foreground
#define VGA_DEFAULT_COLOR (0xF << 8)

// black background, gray foreground, ASCII space
#define VGA_NULLCELL (VGA_DEFAULT_COLOR | ' ')

#ifndef __ASM__
#endif


#endif