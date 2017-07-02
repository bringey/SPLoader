/*
** File: include/SPLoader/console/driver.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_CONSOLE_DRIVER_H
#define _SPLOADER_CONSOLE_DRIVER_H

int con_driver_clear(void);

int con_driver_height(unsigned *heightVar);

int con_driver_put(char ch, unsigned x, unsigned y);

int con_driver_scroll(unsigned lines);

int con_driver_updateCursor(unsigned x, unsigned y);

int con_driver_width(unsigned *widthVar);

#endif