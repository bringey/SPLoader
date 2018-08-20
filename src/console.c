/*
** File: src/core/console.c
**
** Author: Brennan Ringey
**
*/

#include <err.h>
#include <console.h>
#include <printf.h>
#include <string.h>

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


static int __putchar(void *opt, char ch);

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
    spl_puts(__putchar, &opt, str);

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
    spl_puts(__putchar, &opt, str);

    return E_SUCCESS;
}

int spl_con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    PutcharOptions opt = {
        .useCursor = true
    };
    spl_vprintf(__putchar, &opt, fmt, args);

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
    spl_vprintf(__putchar, &opt, fmt, args);

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


int __putchar(void *data, char ch) {

    PutcharOptions *opt = (PutcharOptions*)data;
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

    return 0;
}
