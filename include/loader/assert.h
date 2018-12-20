/*
** File: include/loader/assert.h
**
** Author: bringey
*/

#ifndef _LOADER_ASSERT_H
#define _LOADER_ASSERT_H

#ifndef __ASM__

#include <loader/abort.h>
#include <loader/err.h>

#ifdef NDEBUG

#define ldr_assert(condition)

#else

#define ldr_assert(condition) \
    if (!(condition)) ldr_except(EX_ASSERT)

#endif


#endif



#endif