/*
** File: include/SPLoader/abort.h
**
** Author: bringey
**
** Functions for aborting the boot process. Aborts occur when an unrecoverable
** error occurs or when a check/assertion failed. A message is printed to the
** user via console and the system is halted.
**
*/

#ifndef _ABORT_H
#define _ABORT_H

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
// Exception occurred, print errno and abort.
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