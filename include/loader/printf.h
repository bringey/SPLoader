/*
** File: include/loader/printf.h
**
** Author: bringey
*/

#ifndef _LOADER_PRINTF_H
#define _LOADER_PRINTF_H


#ifndef __ASM__

#include <stdarg.h>


typedef int (*PutcharFn)(void *opt, char ch);

int ldr_puts(PutcharFn putcharFn, void *opt, const char *str);

int ldr_printf(PutcharFn putcharFn, void *opt, const char *fmt, ...);

int ldr_vprintf(PutcharFn putcharFn, void *opt, const char *fmt, va_list args);

#endif  // __ASM__

#endif