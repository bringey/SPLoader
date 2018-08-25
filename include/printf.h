/*
** File: include/printf.h
**
** Author: bringey
*/

#ifndef _PRINTF_H
#define _PRINTF_H


#ifndef __ASM__

#include <stdarg.h>


typedef int (*PutcharFn)(void *opt, char ch);

int spl_puts(PutcharFn putcharFn, void *opt, const char *str);

int spl_printf(PutcharFn putcharFn, void *opt, const char *fmt, ...);

int spl_vprintf(PutcharFn putcharFn, void *opt, const char *fmt, va_list args);

#endif  // __ASM__

#endif