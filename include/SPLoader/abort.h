/*
** File: include/SPLoader/abort.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_ABORT_H
#define _SPLOADER_ABORT_H

#ifndef __ASM__

#include <stdnoreturn.h>

#ifdef DEBUG_FILENAMES
#define spl_error(errno, msg) _spl_error(__FILE__, __LINE__, errno, msg)
#else
#define spl_error(errno, msg) _spl_error(errno, msg)
#endif

#define spl_abort() _spl_abort()

noreturn void _spl_error(
    #ifdef DEBUG_FILENAMES
        const char *file,
        unsigned line,
    #endif
        int errno,
        const char *msg
);

noreturn void _spl_abort(void);

#endif  // __ASM__

#endif