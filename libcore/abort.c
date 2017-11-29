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
        int errno,
        const char *msg
) {
    // print "[ERROR] [(<errno>)] <msg>"
    // errno is not printed if errno == E_UNSPECIFIED
    spl_con_puts("[ERROR] ");

    #ifndef NDEBUG
        if (errno == E_ASSERT) {
            msg = "assertion has failed";
        }
    #endif

    if (errno != E_UNSPECIFIED) {
        spl_con_printf("(%d) ", errno);
    }
    spl_con_printf("%s\n", msg);

    #ifdef DEBUG_FILENAMES
        spl_con_printf("[ERROR] at %s:%d\n", file, line);
    #endif

    _spl_abort();
}
