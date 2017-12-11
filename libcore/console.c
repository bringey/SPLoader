/*
** File: loader/console.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/err.h>
#include <SPLoader/console.h>
#include <SPLoader/console/driver.h>
#include <SPLoader/console/string.h>
#include <SPLoader/string.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned curX, curY;
static unsigned curIndex;
static unsigned curColor;
static unsigned WIDTH, HEIGHT;
static unsigned scrollMinY, scrollMaxY;



typedef struct PutcharOptions_s {
    bool useCursor;      // should we put at the cursor location or a specified
    unsigned x;          // coordinate pair
    unsigned y;
    unsigned index;      // index
} PutcharOptions;


static void __putchar(PutcharOptions *opt, char ch);
static void __puts(PutcharOptions *opt, const char *str);
static void __printf(PutcharOptions *opt, const char *fmt, va_list args);
static void __pad(PutcharOptions *opt, int extra, char padchar);
static void __padstr(PutcharOptions *opt,
                     char *str,
                     unsigned len,
                     unsigned width,
                     bool leftadjust,
                     int padchar);

int spl_con_clear(void) {
    curX = 0;
    curY = scrollMinY;
    curIndex = _spl_con_index(curX, curY);
    _spl_con_clearRegion(0, HEIGHT); // clear the entire screen
    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_clearWindow(void) {
    curX = 0;
    curY = scrollMinY;
    curIndex = _spl_con_index(curX, curY); // clear the window
    _spl_con_clearRegion(scrollMinY, scrollMaxY);
    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_init(void) {

    curX = 0;
    curY = 0;
    curIndex = _spl_con_index(0, 0);
    curColor = _spl_con_color(CON_DEFAULT_FG, CON_DEFAULT_BG);

    WIDTH = _spl_con_width();
    HEIGHT = _spl_con_height();

    scrollMinY = 0;
    scrollMaxY = HEIGHT;

    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_putchar(char ch) {

    PutcharOptions opt = {
        .useCursor = true
    };
    __putchar(&opt, ch);

    return E_SUCCESS;
}

int spl_con_putchar_at(unsigned x, unsigned y, char ch) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _spl_con_index(x, y)
    };
    __putchar(&opt, ch);

    return E_SUCCESS;
}

int spl_con_puts(const char *str) {

    PutcharOptions opt = {
        .useCursor = true
    };
    __puts(&opt, str);

    return E_SUCCESS;
}

int spl_con_puts_at(unsigned x, unsigned y, const char *str) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }
    
    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _spl_con_index(x, y)
    };
    __puts(&opt, str);

    return E_SUCCESS;
}

int spl_con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    PutcharOptions opt = {
        .useCursor = true
    };
    __printf(&opt, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

int spl_con_printf_at(unsigned x, unsigned y, const char *fmt, ...) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    va_list args;
    va_start(args, fmt);

    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _spl_con_index(x, y)
    };
    __printf(&opt, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

int spl_con_scroll(unsigned lines) {
    return _spl_con_scroll(scrollMinY, scrollMaxY, lines);
}

int spl_con_setColor(unsigned color) {
    curColor = color;
    return E_SUCCESS;
}

int spl_con_setCursor(unsigned x, unsigned y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    curX = x;
    curY = y;
    curIndex = _spl_con_index(x, y);
    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_setWindow(unsigned minY, unsigned maxY) {
    if (minY >= maxY || maxY > HEIGHT) {
        return E_ARGBOUNDS;
    }

    scrollMinY = minY;
    scrollMaxY = maxY;
    curX = 0;
    curY = minY;
    curIndex = _spl_con_index(0, minY);
    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

// ============================================================================
// STATIC FUNCTIONS
// ============================================================================


void __putchar(PutcharOptions *opt, char ch) {

    bool useCursor = opt->useCursor;

    unsigned x, y, index;
    if (useCursor) {
        // scroll if needed
        if (curY == scrollMaxY) {
            _spl_con_scroll(scrollMinY, scrollMaxY, 1);
            --curY;
            curIndex -= WIDTH;
        }
        x = curX;
        y = curY;
        index = curIndex;
    } else {
        x = opt->x;
        y = opt->y;
        index = opt->index;
    }

    switch (ch) {
        case '\n':
            for (; x != WIDTH; ++x, ++index) {
                _spl_con_put(index, curColor, ' ');
            }
            x = 0;
            ++y;
            break;
        case '\r':
            x = 0;
            index = _spl_con_index(x, y);
            break;
        default:
            _spl_con_put(index, curColor, ch);
            ++index;
            if (++x == WIDTH) {
                x = 0;
                ++y;
            }
            break;
    }

    if (useCursor) {
        curX = x;
        curY = y;
        curIndex = index;
        _spl_con_updateCursor(index);
    } else {
        opt->x = x;
        opt->y = y;
        opt->index = index;
    }
}

void __puts(PutcharOptions *opt, const char *str) {
    char ch;
    while ((ch = *str++) != '\0') {
        __putchar(opt, ch);
    }
}

void __pad(PutcharOptions *opt, int extra, char padchar) {
    for (; extra > 0; --extra) {
        __putchar(opt, padchar);
    }
}

void __padstr(PutcharOptions *opt, char *str, unsigned len, unsigned width, bool leftadjust, int padchar) {
    int extra = width - len;

    // pad if needed for right-adjust
    if (extra > 0 && !leftadjust) {
        __pad(opt, extra, padchar);
    }
    
    // print the string
    __puts(opt, str);

    // pad if needed for left-adjust
    if (extra > 0 && leftadjust) {
        __pad(opt, extra, padchar);
    }
}



#define PRINTF_BUF_SIZE 80

void __printf(PutcharOptions *opt, const char *fmt, va_list args) {

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
                    len = spl_con_cvtdec(buf, (int)va_arg(args, int));
                    break;
                case 'o':       // octal integer
                    len = spl_con_cvtoct(buf, (unsigned)va_arg(args, unsigned));
                    break;
                case 's':       // string
                    str = (char*)va_arg(args, char*);
                    len = spl_strlen(str);
                    break;
                case 'x':       // hexadecimal integer (lowercase)
                case 'X':       // hexadecimal integer (uppercase)
                    len = spl_con_cvthex(buf, (unsigned)va_arg(args, unsigned));
                    break;
                default:
                    typeValid = false;
                    break;
            }

            if (typeValid) {
                __padstr(opt, str, len, width, leftadjust, padchar);
            }



        } else {
            __putchar(opt, ch);
        }
    }
}
