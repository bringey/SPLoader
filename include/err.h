/*
** File: include/SPLoader/err.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_ERR_H
#define _SPLOADER_ERR_H


//
// When adding new errnos:
//  * Add a macro define in the #ifdef __ASM__ block
//  * Add the errno in the ERR_FOREACH macro list
//  * Macro definition MUST match position in list
//  * Order matters, try to maintain current order by adding to end of list
//

#ifdef __ASM__

//
// Success, no error
//
#define E_SUCCESS 0

// General errors, if passed in a call to spl_except(errno), the name of the
// error will be printed along with the number.

//
// Generic error
//
#define E_FAILURE 1

//
// A passed argument was null
//
#define E_ARGNULL 2

//
// A passed argument is out of bounds
//
#define E_ARGBOUNDS 3

//
// An assertion check has failed
//
#define E_ASSERT 4


//
// Unable to allocate memory, no memory available
//
#define E_NOMEM 5

#else

#define ERR_FOREACH(m) \
    m(E_SUCCESS) \
    m(E_FAILURE) \
    m(E_ARGNULL) \
    m(E_ARGBOUNDS) \
    m(E_ASSERT) \
    m(E_NOMEM)

#define ERR_ENUM(err) err, 
#define ERR_STR(err) #err,

enum {
    ERR_FOREACH(ERR_ENUM)
    // ERR_FOREACH leaves a trailing comma
};

//
// Table of errno names, to get name: ERR_NAMES[errno]
//
extern const char *ERR_NAMES[];

//
// Length of the ERR_NAMES TABLE
//
extern const unsigned ERR_COUNT;


#endif

#endif