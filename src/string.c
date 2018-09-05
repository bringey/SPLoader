/*
** File: src/string.c
**
** Author: bringey
**
*/

#include <assert.h>
#include <err.h>
#include <string.h>

#include <stdint.h>


static int __ultostr(unsigned long u, unsigned base, char *buf);

int ltostr(long i, unsigned base, char *buf) {
    assert(buf != NULL);

    unsigned long value;

    if (i < 0) {
        value = -i;
        // only base 10 integers get prefixed with '-' if negative
        if (base == 10) {
            *buf++ = '-';
        }
    } else {
        value = (unsigned long)i;
    }
    return __ultostr(value, base, buf);
}

int ultostr(unsigned long u, unsigned base, char *buf) {
    assert(buf != NULL);
    return __ultostr(u, base, buf);
}

void* memcpy(void *dest, const void *source, size_t num) {
    uint8_t *dbuf = (uint8_t*)dest;
    uint8_t *sbuf = (uint8_t*)source;
    for (size_t i = 0; i != num; ++i) {
        dbuf[i] = sbuf[i];
    }

    return dest;
}

int strlower(char *str) {
    assert(str != NULL);

    char ch;
    while ((ch = *str) != '\0') {
        // test if ch is lowercase [a-z]
        if (isupper(ch)) {
            // convert ch to uppercase and store it in the string
            // fyi ('a' - 'A') => 32
            *str = tolower(ch);
        }
        ++str;
    }

    return E_SUCCESS;
}

int strupper(char *str) {
    assert(str != NULL);

    char ch;
    while ((ch = *str) != '\0') {
        // test if ch is lowercase [a-z]
        if (islower(ch)) {
            // convert ch to uppercase and store it in the string
            // fyi ('a' - 'A') => 32
            *str = toupper(ch);
        }
        ++str;
    }

    // no errors possible, always returns E_SUCCESS
    return E_SUCCESS;
}

size_t strlen(const char *str) {
    unsigned len = 0;
    while (*str++ != '\0') {
        ++len;
    }
    return len;
}

int __ultostr(unsigned long val, unsigned base, char *buf) {

    char *bp = buf;
    do {
        char c = val % base + '0';
        val /= base;
        if (c >= ':') {
            // skip to the alphabet for digits past 9
            c += 'A' - ':';
        }
        *bp++ = c;
    } while (val != 0);

    // reverse the buffer
    // two pointers: s1 and s2
    // s1 points to the first character in the buffer
    // s2 points to the last character in the buffer
    // the pointers exchange values, s1 is incremented and s2 decremented
    // the buffer is reversed when the pointers "meet in the middle" or when s2
    // is no longer greater than s1
    char temp;
    for (char *s1 = buf, *s2 = bp - 1; s2 > s1; ++s1, --s2) {
        temp = *s1;
        *s1 = *s2;
        *s2 = temp;
    }

    // terminate the buffer
    *bp = '\0';
    // return size of the buffer
    return bp - buf;

}
