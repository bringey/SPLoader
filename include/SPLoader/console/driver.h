/*
** File: include/SPLoader/console/driver.h
**
** Author: Brennan Ringey
**
** Platform specific functions for the console. When porting to different
** platforms only the functions defined in this header must be implemented for
** the console.
*/

#ifndef _SPLOADER_CONSOLE_DRIVER_H
#define _SPLOADER_CONSOLE_DRIVER_H

#include <SPLoader/console/ConColor.h>

#ifndef __ASM__

/*
** @brief clear function for the console driver
**
** Clears the on-screen console buffer.
*/
int con_driver_clear(void);

/*
** @brief Gets the height, in rows, of the console.
**
** Stores the current console height, in rows, in the given unsigned pointer.
**
*/
int con_driver_height(unsigned *heightVar);

/*
** @brief Puts a character at the given row and column 
*/
int con_driver_put(char ch, unsigned x, unsigned y);

//int con_driver_scroll(unsigned lines);

int con_driver_scroll(unsigned lineStart, unsigned lineEnd, unsigned lines);

int con_driver_setBgColor(ConColor color);

int con_driver_setFgColor(ConColor color);

int con_driver_updateCursor(unsigned x, unsigned y);

int con_driver_width(unsigned *widthVar);

#endif  // __ASM__

#endif