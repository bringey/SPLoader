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
** @brief Puts a character at the given row and column 
*/
int con_driver_put(char ch, unsigned x, unsigned y);

int con_driver_scroll(unsigned lineStart, unsigned lineEnd, unsigned lines);

int con_driver_setBgColor(ConColor color);

int con_driver_setFgColor(ConColor color);

int con_driver_updateCursor(unsigned x, unsigned y);

// driver properties

unsigned con_driver_columnOffset(void);

unsigned con_driver_rowOffset(void);

unsigned con_driver_height(void);

unsigned con_driver_width(void);


#endif  // __ASM__

#endif