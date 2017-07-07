/*
** File: loader/console/out.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/err.h>
#include <SPLoader/console/out.h>
#include <SPLoader/console/driver.h>
#include <SPLoader/console/string.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned curX, curY;
static unsigned WIDTH, HEIGHT;


//
// Typedef for a putchar function
// Two putchar functions are defined:
//   __putchar:   writes a char at (curX,curY) to the console
//   con_putchar: same as __putchar, but updates the cursor location
//
typedef int (*PutcharFn)(char);

static int __putchar(char ch);
static void __puts(PutcharFn fn, const char *str);
static void __printf(PutcharFn fn, const char *fmt, va_list args);
static void __pad(PutcharFn fn, int extra, char padchar);
static void __padstr(PutcharFn fn,
                     char *str,
                     unsigned len,
                     unsigned width,
                     bool leftadjust,
                     int padchar);

int con_clear(void) {
    curX = 0;
    curY = 0;
    con_driver_clear();
    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}

int con_init(void) {

    curX = 0;
    curY = 0;

    con_driver_width(&WIDTH);
    con_driver_height(&HEIGHT);

    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}

int con_putchar(char ch) {

    __putchar(ch);
    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}

int con_putchar_at(unsigned x, unsigned y, char ch) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }
    unsigned oldX = curX, oldY = curY;

    curX = x;
    curY = y;
    __putchar(ch);
    curX = oldX;
    curY = oldY;

    return E_SUCCESS;
}

int con_puts(const char *str) {

    __puts(con_putchar, str);

    return E_SUCCESS;
}

int con_puts_at(unsigned x, unsigned y, const char *str) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }
    unsigned oldX = curX, oldY = curY;

    curX = x;
    curY = y;
    __puts(__putchar, str);
    curX = oldX;
    curY = oldY;

    return E_SUCCESS;
}

int con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __printf(con_putchar, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

int con_printf_at(unsigned x, unsigned y, const char *fmt, ...) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    va_list args;
    va_start(args, fmt);

    unsigned oldX = curX, oldY = curY;

    curX = x;
    curY = y;
    __printf(__putchar, fmt, args);
    curX = oldX;
    curY = oldY;

    va_end(args);
    return E_SUCCESS;
}

#ifndef con_scroll
int con_scroll(unsigned lines) {
    return con_driver_scroll(lines);
}
#endif

#ifndef con_setBgColor
int con_setBgColor(ConColor color) {
    return con_driver_setBgColor(color);
}
#endif

#ifndef con_setFgColor
int con_setFgColor(ConColor color) {
    return con_driver_setFgColor(color);
}
#endif

int con_setCursor(unsigned x, unsigned y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    curX = x;
    curY = y;
    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}

// ============================================================================
// STATIC FUNCTIONS
// ============================================================================


int __putchar(char ch) {

    if (curY == HEIGHT) {
        con_driver_scroll(1);
        --curY;
    }

    switch (ch) {
        case '\n':
            for (; curX != WIDTH; ++curX) {
                con_driver_put(' ', curX, curY);
            }
            curX = 0;
            ++curY;
            break;
        case '\r':
            curX = 0;
            break;
        default:
            con_driver_put(ch, curX, curY);
            if (++curX == WIDTH) {
                curX = 0;
                ++curY;
            }
            break;
    }

    return 0;
}

void __puts(PutcharFn putchar, const char *str) {
    char ch;
    while ((ch = *str++) != '\0') {
        putchar(ch);
    }
}

void __pad(PutcharFn putchar, int extra, char padchar) {
    for (; extra > 0; --extra) {
        putchar(padchar);
    }
}

void __padstr(PutcharFn putchar, char *str, unsigned len, unsigned width, bool leftadjust, int padchar) {
    int extra = width - len;

    // pad if needed for right-adjust
    if (extra > 0 && !leftadjust) {
        __pad(putchar, extra, padchar);
    }
    
    // print the string
    __puts(putchar, str);

    // pad if needed for left-adjust
    if (extra > 0 && leftadjust) {
        __pad(putchar, extra, padchar);
    }
}



#define PRINTF_BUF_SIZE 80

void __printf(PutcharFn putchar, const char *fmt, va_list args) {

    bool leftadjust, typeValid;
    char buf[PRINTF_BUF_SIZE];
    char *str;
    unsigned width, len;
    char padchar;

    char ch;
    while ((ch = *fmt++) != '\0') {
        if (ch == '%') {
            // defaults
            // right-adjusted, space-padded, no width constraint
            leftadjust = false;
            padchar = ' ';
            width = 0;
            str = buf;
            typeValid = true;
            ch = *fmt++; // get next part of fmt and consume it
            if (ch == '-') {
                leftadjust = true; // left alignment
                ch = *fmt++;
            }
            if (ch == '0') {
                padchar = '0';  // pad with zeros instead of spaces
                ch = *fmt++;
            }
            // get the width if specified
            while ( ch >= '0' && ch <= '9') {
                width *= 10;
                width += ch - '0';
                ch = *fmt++;
            }

            switch (ch) {
                case 'c':       // character
                    buf[0] = (char)va_arg(args, int);
                    buf[1] = '\0';
                    len = 1;
                    break;
                case 'd':       // decimal integer
                    len = con_cvtdec(buf, (int)va_arg(args, int));
                    break;
                case 'o':       // octal integer
                    len = con_cvtoct(buf, (unsigned)va_arg(args, unsigned));
                    break;
                case 's':       // string
                    str = (char*)va_arg(args, char*);
                    len = con_strlen(str);
                    break;
                case 'x':       // hexadecimal integer (lowercase)
                case 'X':       // hexadecimal integer (uppercase)
                    len = con_cvthex(buf, (unsigned)va_arg(args, unsigned));
                    break;
                default:
                    typeValid = false;
                    break;
            }

            if (typeValid) {
                __padstr(putchar, str, len, width, leftadjust, padchar);
            }



        } else {
            putchar(ch);
        }
    }
}