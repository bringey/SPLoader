/*
** File: loader/console/string.c
**
** Author: Brennan Ringey
**
*/

#if defined(FE_MENU) || defined(FE_CONSOLE)

#include <SPLoader/console/string.h>


static char HEXDIGITS[] = "0123456789ABCDEF";

static char* spl_con_cvtdec0(char *buf, int value) {
    int quotient = value / 10;
    if (quotient < 0) {
        quotient = 214748364;
        value = 8;
    }

    if (quotient != 0) {
        buf = spl_con_cvtdec0(buf, quotient);
    }
    *buf++ = value % 10 + '0';
    return buf;
}

int spl_con_cvtdec(char *buf, int value) {
    char *bp = buf;
    if (value < 0) {
        *bp++ = '-';
        value = -value;
    }

    bp = spl_con_cvtdec0(bp, value);
    *bp = '\0';

    return bp - buf;
}

int spl_con_cvthex(char *buf, int value) {

    int chars_stored = 0;
    char *bp = buf;

    for (int i = 0; i < 8; ++i) {
        int val = value & 0xF0000000;
        if ( i == 7 || val != 0 || chars_stored) {
            chars_stored = 1;
            val >>= 28;
            val &= 0xF;
            *bp++ = HEXDIGITS[val];
        }
        value <<= 4;
    }
    *bp = '\0';

    return bp - buf;
}

int spl_con_cvtoct(char *buf, int value) {
    int chars_stored = 0;
    char *bp = buf;
    
    int val = value & 0xc0000000;
    val >>= 30;
    for (int i = 0; i < 11; ++i) {
        if (i == 10 || val != 0 || chars_stored) {
            chars_stored = 1;
            val &= 0x7;
            *bp++ = HEXDIGITS[val];
        }
        value <<= 3;
        val = value & 0xE0000000;
        val >>= 29;
    }
    *bp = '\0';

    return bp - buf;
}

unsigned spl_con_strlen(char *str) {
    unsigned len = 0;
    while (*str++ != '\0') {
        ++len;
    }
    return len;
}

#else
typedef int dummy;
#endif
