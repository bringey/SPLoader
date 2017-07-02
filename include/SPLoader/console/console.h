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

int con_puts(const char *str);

int con_setCursor(unsigned x, unsigned y);


#endif