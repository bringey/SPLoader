/*
** File: src/console.c
**
** Author: bringey
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
static unsigned WIDTH, HEIGHT;



static int __putchar(void *opt, char ch);

int spl_con_clear(void) {
    curX = 0;
    curY = 0;
    curIndex = _spl_con_index(curX, curY);
    _spl_con_clear();
    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_init(void) {

    curX = 0;
    curY = 0;
    curIndex = _spl_con_index(0, 0);

    WIDTH = _spl_con_width();
    HEIGHT = _spl_con_height();

    _spl_con_updateCursor(curIndex);

    return E_SUCCESS;
}

int spl_con_putchar(char ch) {

    __putchar(NULL, ch);

    return E_SUCCESS;
}

int spl_con_puts(const char *str) {

    spl_puts(__putchar, NULL, str);

    return E_SUCCESS;
}

int spl_con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    spl_vprintf(__putchar, NULL, fmt, args);

    va_end(args);
    return E_SUCCESS;
}

// ============================================================================
// STATIC FUNCTIONS
// ============================================================================


int __putchar(void *data, char ch) {

    (void)data;

    // scroll if needed
    if (curY == HEIGHT) {
        _spl_con_scroll(1);
        --curY;
    }

    switch (ch) {
        case '\n':
            for (; curX != WIDTH; ++curX, ++curIndex) {
                _spl_con_put(curIndex, ' ');
            }
            curX = 0;
            ++curY;
            break;
        case '\r':
            // carriage-return: move cursor back to start of line
            curX = 0;
            curIndex = _spl_con_index(curX, curY);
            break;
        default:
            _spl_con_put(curIndex, ch);
            ++curIndex;
            if (++curX == WIDTH) {
                curX = 0;
                ++curY;
            }
            break;
    }

    if (curY == HEIGHT) {
        // don't scroll just yet, but keep the cursor on the screen
        curIndex -= WIDTH;
    }

    _spl_con_updateCursor(curIndex);

    return 0;
}
