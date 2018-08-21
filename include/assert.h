
#ifndef _SPLOADER_ASSERT_H
#define _SPLOADER_ASSERT_H

#include <abort.h>
#include <err.h>
#include <stddef.h>

#ifndef __ASM__

#define spl_checke(condition, errno) \
    if (!(condition)) spl_except(errno)

#define spl_check(condition, msg) \
    if (!(condition)) spl_error(msg)

// asserts are the same as check, but checks will always exist in compiled
// output. Use assert for debugging, use check for sanity
#ifdef NDEBUG
#define spl_assert(condition)
#define spl_asserte(condition, errno)
#else
#define spl_assert(condition) spl_checke(condition, E_ASSERT)
#define spl_asserte(condition, errno) spl_checke(condition, errno)
#endif


#endif



#endif