/*
** File: include/assert.h
**
** Author: bringey
*/

#ifndef _ASSERT_H
#define _ASSERT_H

#include <abort.h>
#include <err.h>
#include <stddef.h>

#ifndef __ASM__

#define checke(condition, errno) \
    if (!(condition)) except(errno)

#define check(condition, msg) \
    if (!(condition)) error(msg)

// asserts are the same as check, but checks will always exist in compiled
// output. Use assert for debugging, use check for sanity
#ifdef NDEBUG
#define assert(condition)
#define asserte(condition, errno)
#else
#define assert(condition) checke(condition, E_ASSERT)
#define asserte(condition, errno) checke(condition, errno)
#endif


#endif



#endif