/*
** File: include/assert.h
**
** Author: bringey
*/

#ifndef _ASSERT_H
#define _ASSERT_H

#ifndef __ASM__

#include <abort.h>
#include <err.h>

#ifdef NDEBUG

#define assert(condition)

#else

#define assert(condition) \
    if (!(condition)) except(EX_ASSERT)

#endif


#endif



#endif