/*
** File: include/SPLoader/err.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_ERR_H
#define _SPLOADER_ERR_H

//
// unknown error
//
#define E_UNSPECIFIED -1

//
// Success, no error
//
#define E_SUCCESS 0

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
#define E_NOMEM 100

#ifndef __ASM__
#endif

#endif