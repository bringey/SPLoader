/*
** File: include/SPLoader/console/driver.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_CONSOLE_DRIVER_H
#define _SPLOADER_CONSOLE_DRIVER_H

#include <SPLoader/console/ConColor.h>

#ifndef __ASM__

int con_driver_clear(void);

int con_driver_height(unsigned *heightVar);

int con_driver_put(char ch, unsigned x, unsigned y);

int con_driver_scroll(unsigned lines);

int con_driver_setBgColor(ConColor color);

int con_driver_setFgColor(ConColor color);

int con_driver_updateCursor(unsigned x, unsigned y);

int con_driver_width(unsigned *widthVar);

#endif  // __ASM__

#endif