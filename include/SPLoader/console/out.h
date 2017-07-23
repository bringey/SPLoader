/*
** File: include/SPLoader/console/out.h
**
** Author: Brennan Ringey
**
** Contains functions for console output.
*/

#ifndef _SPLOADER_CONSOLE_OUT_H
#define _SPLOADER_CONSOLE_OUT_H

#include <SPLoader/console/ConColor.h>
#include <SPLoader/console/driver.h>

#define CON_DEFAULT_FG CON_COLOR_WHITE
#define CON_DEFAULT_BG CON_COLOR_BLACK

#ifndef __ASM__

int con_clear(void);

int con_init(void);

int con_putchar(char ch);

int con_putchar_at(unsigned x, unsigned y, char ch);

int con_puts(const char *str);

int con_puts_at(unsigned x, unsigned y, const char *str);

int con_printf(const char *fmt, ...);

int con_printf_at(unsigned x, unsigned y, const char *fmt, ...);

//int con_scroll(unsigned lines);
#define con_scroll(lines) con_driver_scroll(lines)

#define con_setBgColor(color) con_driver_setBgColor(color)

#define con_setFgColor(color) con_driver_setFgColor(color)

int con_setCursor(unsigned x, unsigned y);

#endif  //__ASM__

#endif