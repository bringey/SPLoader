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


void spl_error_(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        const char *msg
) {
    spl_con_printf("Error occurred: %s\n", msg);
    #ifdef DEBUG_FILENAMES
        spl_con_printf("[ERROR] at %s:%d\n", file, line);
    #endif

    _spl_abort();
}

void spl_except_(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        int errno
) {

    spl_con_printf("Error occurred. Code: %d\n", errno);
    #ifdef DEBUG_FILENAMES
    spl_con_printf("%s:%d\n", file, line);
    #endif

    _spl_abort();

}
