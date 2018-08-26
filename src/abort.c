/*
** File: src/abort.c
**
** Author: bringey
**
** Implementations for functions defined in include/abort.h
**
*/

#include <abort.h>
#include <err.h>
#include <console.h>


void error_(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        const char *msg
) {
    con_printf("Error occurred: %s\n", msg);
    #ifdef DEBUG_FILENAMES
        con_printf("[ERROR] at %s:%d\n", file, line);
    #endif

    _abort();
}

void except_(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        int errno
) {

    con_printf("Error occurred. Code: %d\n", errno);
    #ifdef DEBUG_FILENAMES
    con_printf("%s:%d\n", file, line);
    #endif

    _abort();

}
