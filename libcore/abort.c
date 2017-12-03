/*
** File: core/i386-pc/abort.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/abort.h>
#include <SPLoader/err.h>
#include <SPLoader/console.h>


void spl_error_(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        const char *msg
) {
    spl_con_printf("[ERROR] %s\n", msg);
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

    const char *errname = "Unknown errno";
    if (errno >= 0 && (unsigned)errno < ERR_COUNT) {
        errname = ERR_NAMES[errno];
    }

    spl_con_printf("[ERROR] %s (%d)\n", errname, errno);
    #ifdef DEBUG_FILENAMES
    spl_con_printf("[ERROR] at %s:%d\n", file, line);
    #endif

    _spl_abort();

}
