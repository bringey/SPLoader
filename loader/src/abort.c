/*
** File: src/abort.c
**
** Author: bringey
**
** Implementations for functions defined in include/abort.h
**
*/

#include <loader/abort.h>
#include <loader/err.h>
#include <loader/console.h>

#include <stdbool.h>

static const char *EX_TABLE[] = {
    "Assertion failed",
    "Stage 1 header error",
    "A memory allocation failed",
    "Disk error",
    "Disk read error",
    "Disk label is invalid or corrupted",
    "Disk partition error"
    // "Disk label is unknown",
    // "Could not find boot partition",
    // "Filesystem error",
    // "Filesystem is invalid or corrupted",
    // "Could not mount filesystem",
    // "Filesystem is unsupported",
    // "Could not read file",
    // "Path does not exist",
    // "Config file does not exist",
    // "Syntax error in config file",
    // "Assertion failed"
};

#define EX_TABLE_SIZE (sizeof(EX_TABLE) / sizeof(const char *))

void ldr_exceptv(unsigned ex, int code) {
    const char *str;
    if (ex >= EX_TABLE_SIZE) {
        str = "unknown";
    } else {
        str = EX_TABLE[ex];
    }

    ldr_con_puts("Error occurred: ");
    ldr_con_printf("%s (%d:%d)\n", str, ex, code);
    _ldr_abort();
}
