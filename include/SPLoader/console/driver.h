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
** Clears a region of the on-screen console buffer. The lines from
** rowStart to rowEnd will be cleared. rowEnd must be greater than
** rowStart.
*/
int _spl_con_clearRegion(unsigned rowStart, unsigned rowEnd);

/*
** @brief calculates a color value for the given ConColor
**
*/
unsigned _spl_con_color(ConColor fg, ConColor bg);

/*
** @brief Calculates an index in the console buffer for the given coordinates
*/
unsigned _spl_con_index(unsigned x, unsigned y);

/*
** @brief Puts a character at the given row and column 
*/
int _spl_con_put(unsigned index, unsigned color, char ch);

int _spl_con_scroll(unsigned lineStart, unsigned lineEnd, unsigned lines);

int _spl_con_updateCursor(unsigned index);

// driver properties

unsigned _spl_con_height(void);

unsigned _spl_con_width(void);


#endif  // __ASM__

#endif