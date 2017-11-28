
#ifndef _SPLOADER_ASSERT_H
#define _SPLOADER_ASSERT_H

#include <SPLoader/abort.h>
#include <SPLoader/err.h>
#include <stddef.h>

#ifndef __ASM__

// check with errno, error message
#define spl_checkem(condition, err, msg) \
    if (!(condition)) spl_error(err, msg)

// check with errno
#define spl_checke(condition, err) spl_checkem(condition, err, NULL)

// check with error message
#define spl_checkm(condition, msg) spl_checkem(condition, E_UNSPECIFIED, msg)

// check with no message and no errno
#define spl_check(condition) spl_checkem(condition, E_UNSPECIFIED, NULL)


// asserts are the same as check, but checks will always exist in compiled
// output. Use assert for debugging, use check for sanity
#ifdef NDEBUG
#define spl_assert(condition) spl_checke(condition, E_ASSERT)
#else
#define spl_assert(condition)
#endif


#endif



#endif