/*
** File: test/src/loader/common/abort.c
**
** Author: bringey
**
** Implementation for abort, calls stdlib exit instead of halting
*/

#include <loader/abort.h>

#include <stdlib.h>

void _loader_abort(void) {
    exit(EXIT_FAILURE);
}