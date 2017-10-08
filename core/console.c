/*
** File: core/console.c
**
** Author: Brennan Ringey
**
*/

#if defined(FE_MENU) || defined(FE_CONSOLE)

#include <SPLoader/err.h>
#include <SPLoader/console.h>
#include <SPLoader/console/driver.h>
#include <SPLoader/console/string.h>

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

int con_clear(void) {
    curX = 0;
    curY = scrollMinY;
    curIndex = _con_index(curX, curY);
    _con_clearRegion(0, HEIGHT); // clear the entire screen
    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_clearWindow(void) {
    curX = 0;
    curY = scrollMinY;
    curIndex = _con_index(curX, curY); // clear the window
    _con_clearRegion(scrollMinY, scrollMaxY);
    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_init(void) {

    curX = 0;
    curY = 0;
    curIndex = _con_index(0, 0);
    curColor = _con_color(CON_DEFAULT_FG, CON_DEFAULT_BG);

    WIDTH = _con_width();
    HEIGHT = _con_height();

    scrollMinY = 0;
    scrollMaxY = HEIGHT;

    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_putchar(char ch) {

    PutcharOptions opt = {
        .useCursor = true
    };
    __putchar(&opt, ch);

    return E_SUCCESS;
}

int con_putchar_at(unsigned x, unsigned y, char ch) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _con_index(x, y)
    };
    __putchar(&opt, ch);

    return E_SUCCESS;
}

int con_puts(const char *str) {

    PutcharOptions opt = {
        .useCursor = true
    };
    __puts(&opt, str);

    return E_SUCCESS;
}

int con_puts_at(unsigned x, unsigned y, const char *str) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }
    
    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _con_index(x, y)
    };
    __puts(&opt, str);

    return E_SUCCESS;
}

int con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    PutcharOptions opt = {
        .useCursor = true
    };
    __printf(&opt, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

int con_printf_at(unsigned x, unsigned y, const char *fmt, ...) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    va_list args;
    va_start(args, fmt);

    PutcharOptions opt = {
        .useCursor = false,
        .x = x,
        .y = y,
        .index = _con_index(x, y)
    };
    __printf(&opt, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

int con_scroll(unsigned lines) {
    return _con_scroll(scrollMinY, scrollMaxY, lines);
}

int con_setColor(unsigned color) {
    curColor = color;
    return E_SUCCESS;
}

int con_setCursor(unsigned x, unsigned y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    curX = x;
    curY = y;
    curIndex = _con_index(x, y);
    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_setWindow(unsigned minY, unsigned maxY) {
    if (minY >= maxY || maxY > HEIGHT) {
        return E_ARGBOUNDS;
    }

    scrollMinY = minY;
    scrollMaxY = maxY;
    curX = 0;
    curY = minY;
    curIndex = _con_index(0, minY);
    _con_updateCursor(curIndex);

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
            _con_scroll(scrollMinY, scrollMaxY, 1);
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
                _con_put(index, curColor, ' ');
            }
            x = 0;
            ++y;
            break;
        case '\r':
            x = 0;
            index = _con_index(x, y);
            break;
        default:
            _con_put(index, curColor, ch);
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
        _con_updateCursor(index);
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
                __padstr(opt, str, len, width, leftadjust, padchar);
            }



        } else {
            __putchar(opt, ch);
        }
    }
}

#else
typedef int dummy;
#endif
