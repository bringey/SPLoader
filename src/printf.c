/*
** File: src/printf.c
**
** Author: bringey
*/

#include <assert.h>
#include <err.h>
#include <printf.h>
#include <string.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define PRINTF_BUF_SIZE 80

static int __vprintf(PutcharFn putcharFn, void *opt, const char *fmt, va_list args);

int puts(PutcharFn putcharFn, void *opt, const char *str) {
    asserte(putcharFn != NULL && str != NULL, E_ARGNULL);

    char ch;
    while ((ch = *str++) != '\0') {
        putcharFn(opt, ch);
    }

    return E_SUCCESS;
}


int printf(PutcharFn putcharFn, void *opt, const char *fmt, ...) {
    asserte(putcharFn != NULL && fmt != NULL, E_ARGNULL);

    va_list args;
    va_start(args, fmt);
    int result = __vprintf(putcharFn, opt, fmt, args);
    va_end(args);

    return result;
}

int vprintf(PutcharFn putcharFn, void *opt, const char *fmt, va_list args) {
    asserte(putcharFn != NULL && fmt != NULL, E_ARGNULL);

    return __vprintf(putcharFn, opt, fmt, args);
}


int __vprintf(PutcharFn putcharFn, void *opt, const char *fmt, va_list args) {
    // parameters have already been error-checked before this function is
    // called, no need to check again

    bool leftadjust, typeValid;
    char buf[PRINTF_BUF_SIZE];
    char *str;
    unsigned width, len;
    char padchar;

    char ch;
    while ((ch = *fmt++) != '\0') {
        if (ch == '%') {
            // defaults: right-adjust, space-padded, no width constraint
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

            if (ch == '*') {
                // get the width from args
                width = (unsigned)va_arg(args, unsigned);
                ch = *fmt++;
            } else {
                // get the width if specified
                while ( ch >= '0' && ch <= '9') {
                    width *= 10;
                    width += ch - '0';
                    ch = *fmt++;
                }
            }

            switch (ch) {
                case 'c':       // character
                    buf[0] = (char)va_arg(args, int);
                    buf[1] = '\0';
                    len = 1;
                    break;
                case 'd':       // decimal integer
                    //len = con_cvtdec(buf, (int)va_arg(args, int));
                    len = ltostr((int)va_arg(args, int), 10, buf);
                    break;
                case 'o':       // octal integer
                    len = ultostr((unsigned)va_arg(args, unsigned), 8, buf);
                    break;
                case 's':       // string
                    str = (char*)va_arg(args, char*);
                    len = strlen(str);
                    break;
                case 'x':       // hexadecimal integer (lowercase)
                case 'X':       // hexadecimal integer (uppercase)
                    len = ultostr((unsigned)va_arg(args, unsigned), 16, buf);
                    break;
                default:
                    typeValid = false;
                    break;
            }

            if (typeValid) {
                int extra = width - len;
                // pad if needed for right-adjust
                if (extra > 0 && !leftadjust) {
                    for (; extra > 0; --extra) {
                        putcharFn(opt, padchar);
                    }
                }

                puts(putcharFn, opt, str);

                // pad if needed for left-adjust
                if (extra > 0 && leftadjust) {
                    for (; extra > 0; --extra) {
                        putcharFn(opt, padchar);
                    }
                }
            }

        } else {
            putcharFn(opt, ch);
        }
    
    }

    return E_SUCCESS;
}