/*
** File: console.c
**
** Author: bringey
**
** Wrapper implentations for con_* functions when testing loader code on the
** host. Outputs to stdout using stdio
**
*/

#include <loader/console.h>
#include <loader/err.h>


#include <stdarg.h>
#include <stdio.h>


int con_clear(void) {
    // not implemented
    return E_SUCCESS;
}

int con_init(void) {
    // do nothing
    return E_SUCCESS;
}

int con_putchar(char ch) {
    putchar(ch);
    return E_SUCCESS;
}

int con_puts(const char *str) {
    puts(str);
    return E_SUCCESS;
}

int con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    return E_SUCCESS;
}