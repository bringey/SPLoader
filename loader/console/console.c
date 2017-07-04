
#include <SPLoader/err.h>
#include <SPLoader/console/driver.h>

#include <stdarg.h>
#include <stdint.h>


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


int con_scroll(unsigned lines) {
    return con_driver_scroll(lines);
}

int con_setCursor(unsigned x, unsigned y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    curX = x;
    curY = y;
    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}



int __putchar(char ch) {
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

    if (curY == HEIGHT) {
        con_driver_scroll(1);
        --curY;
    }

    return 0;
}

void __puts(PutcharFn putchar, const char *str) {
    char ch;
    while ((ch = *str++) != '\0') {
        putchar(ch);
    }
}

void __printf(PutcharFn putchar, const char *fmt, va_list args) {
    char ch;
    while ((ch = *fmt++) != '\0') {
        if (ch == '%') {

        } else {
            putchar(ch);
        }
    }
}