/*
** File: include/SPLoader/console/console.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_CONSOLE_CONSOLE_H
#define _SPLOADER_CONSOLE_CONSOLE_H


int con_clear(void);

int con_init(void);

int con_putchar(char ch);

//int con_putchar_at(unsigned x, unsigned y, char ch);

int con_puts(const char *str);

//int con_puts_at(unsigned x, unsigned y, const char *str);

int con_scroll(unsigned lines);

int con_setCursor(unsigned x, unsigned y);


#endif