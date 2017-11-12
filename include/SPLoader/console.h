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

int spl_con_clear(void);

int spl_con_clearWindow(void);

#define spl_con_color(fg, bg) _spl_con_color(fg, bg)

int spl_con_init(void);

int spl_con_putchar(char ch);

int spl_con_putchar_at(unsigned x, unsigned y, char ch);

int spl_con_puts(const char *str);

int spl_con_puts_at(unsigned x, unsigned y, const char *str);

int spl_con_printf(const char *fmt, ...);

int spl_con_printf_at(unsigned x, unsigned y, const char *fmt, ...);

#define spl_con_resetColor() spl_con_setColor(CON_DEFAULT_FG, CON_DEFAULT_BG)

int spl_con_scroll(unsigned lines);

int spl_con_setColor(unsigned color);

int spl_con_setCursor(unsigned x, unsigned y);

int spl_con_setWindow(unsigned minY, unsigned maxY);

#define spl_con_width() _spl_con_width()

#define spl_con_height() _spl_con_height()

#endif  //__ASM__

#endif