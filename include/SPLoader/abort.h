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
#define spl_except(errno) spl_except_(__FILE__, __LINE__, errno)
#define spl_error(msg) spl_error_(__FILE__, __LINE__, msg)
#else
#define spl_except(errno) spl_except_(errno)
#define spl_error(msg) spl_error_(msg)
#endif

#define spl_abort() _spl_abort()

//
// Error occurred, print error message and abort
//
noreturn void spl_error_(
    #ifdef DEBUG_FILENAMES
        const char *file,
        unsigned line,
    #endif
        const char *msg
);

//
// Exception occurred, print exception name and abort. errno must be defined in
// SPLoader/err.h
//
noreturn void spl_except_(
    #ifdef DEBUG_FILENAMES
        const char *file,
        unsigned line,
    #endif
        int errno
);

noreturn void _spl_abort(void);

#endif  // __ASM__

#endif