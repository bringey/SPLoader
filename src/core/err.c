/*
** File: src/core/err.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/err.h>

const char *ERR_NAMES[] = {
    ERR_FOREACH(ERR_STR)
};

const unsigned ERR_COUNT = sizeof(ERR_NAMES) / sizeof(const char *);

