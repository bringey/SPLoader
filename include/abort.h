/*
** File: include/abort.h
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
#define except(errno) except_(__FILE__, __LINE__, errno)
#define error(msg) error_(__FILE__, __LINE__, msg)
#else
#define except(errno) except_(errno)
#define error(msg) error_(msg)
#endif

#define abort() _abort()

//
// Error occurred, print error message and abort
//
noreturn void error_(
    #ifdef DEBUG_FILENAMES
        const char *file,
        unsigned line,
    #endif
        const char *msg
);

//
// Exception occurred, print errno and abort.
//
noreturn void except_(
    #ifdef DEBUG_FILENAMES
        const char *file,
        unsigned line,
    #endif
        int errno
);

noreturn void _abort(void);

#endif  // __ASM__

#endif