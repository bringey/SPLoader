/*
** File: include/SPLoader/console.h
**
** Author: Brennan Ringey
**
** Contains functions for console output.
*/

#ifndef _SPLOADER_CONSOLE_H
#define _SPLOADER_CONSOLE_H

#include <SPLoader/console/ConColor.h>
#include <SPLoader/console/driver.h>

#define CON_DEFAULT_FG CON_COLOR_WHITE
#define CON_DEFAULT_BG CON_COLOR_BLACK

#ifndef __ASM__

int con_clear(void);

int con_clearWindow(void);

#define con_color(fg, bg) _con_color(fg, bg)

int con_init(void);

int con_putchar(char ch);

int con_putchar_at(unsigned x, unsigned y, char ch);

int con_puts(const char *str);

int con_puts_at(unsigned x, unsigned y, const char *str);

int con_printf(const char *fmt, ...);

int con_printf_at(unsigned x, unsigned y, const char *fmt, ...);

#define con_resetColor() con_setColor(CON_DEFAULT_FG, CON_DEFAULT_BG)

int con_scroll(unsigned lines);

int con_setColor(unsigned color);

int con_setCursor(unsigned x, unsigned y);

int con_setWindow(unsigned minY, unsigned maxY);

#define con_width() _con_width()

#define con_height() _con_height()

#endif  //__ASM__

#endif