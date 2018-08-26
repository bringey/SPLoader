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

int con_clear(void) {
    curX = 0;
    curY = 0;
    curIndex = _con_index(curX, curY);
    _con_clear();
    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_init(void) {

    curX = 0;
    curY = 0;
    curIndex = _con_index(0, 0);

    WIDTH = _con_width();
    HEIGHT = _con_height();

    _con_updateCursor(curIndex);

    return E_SUCCESS;
}

int con_putchar(char ch) {

    __putchar(NULL, ch);

    return E_SUCCESS;
}

int con_puts(const char *str) {

    puts(__putchar, NULL, str);

    return E_SUCCESS;
}

int con_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vprintf(__putchar, NULL, fmt, args);

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
        _con_scroll(1);
        --curY;
    }

    switch (ch) {
        case '\n':
            for (; curX != WIDTH; ++curX, ++curIndex) {
                _con_put(curIndex, ' ');
            }
            curX = 0;
            ++curY;
            break;
        case '\r':
            // carriage-return: move cursor back to start of line
            curX = 0;
            curIndex = _con_index(curX, curY);
            break;
        default:
            _con_put(curIndex, ch);
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

    _con_updateCursor(curIndex);

    return 0;
}
