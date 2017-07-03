
#include <SPLoader/err.h>
#include <SPLoader/console/driver.h>
#include <stdint.h>


static unsigned curX, curY;
static unsigned WIDTH, HEIGHT;

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

    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}

// int con_putchar_at(unsigned x, unsigned y, char ch) {
//     if (x >= WIDTH || y >= HEIGHT) {
//         return E_ARGBOUNDS;
//     }

//     switch (ch) {
//         case '\n':
//             for (unsigned i = x; i != WIDTH; ++i) {
//                 con_driver_put(' ', i, y);
//             }
//             break;
//         default:
//             con_driver_put(ch, x, y);
//             break;
//     }

//     return E_SUCCESS;
// }

int con_puts(const char *str) {

    char ch;
    while ((ch = *str++) != '\0') {
        con_putchar(ch);
    }

    return E_SUCCESS;
}

// int con_puts_at(unsigned x, unsigned y, const char *str) {

// }

int con_setCursor(unsigned x, unsigned y) {
    if (x >= WIDTH || y >= HEIGHT) {
        return E_ARGBOUNDS;
    }

    curX = x;
    curY = y;
    con_driver_updateCursor(curX, curY);

    return E_SUCCESS;
}