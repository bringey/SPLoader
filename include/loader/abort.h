/*
** File: include/loader/abort.h
**
** Author: bringey
**
** Functions for aborting the boot process. Aborts occur when an unrecoverable
** error occurs or when a check/assertion failed. A message is printed to the
** user via console and the system is halted.
**
*/

#ifndef _LOADER_ABORT_H
#define _LOADER_ABORT_H

// === EXCEPTIONS =============================================================
// Exceptions are fatal errors that are possible throughout the loader code.
// Each exception has a message associated with it to be displayed to the user
// when it is encountered. Some exceptions have an additional error code to be
// displayed as well (ie EX_DISK_LABEL_INVALID)
// ----------------------------------------------------------------------------

// NOTE: the values for these exceptions will be PERMANENT once version 1.0.0
// is reached, otherwise they are subject to change at any point in time


// Assertion has failed (see assert.h)
#define EX_ASSERT                   0

// Invalid or integrity error for header
#define EX_HEADER                   1

// Memory exception
#define EX_MEM                      2

// General disk exception
#define EX_DISK                     3

// Read error in disk
#define EX_DISK_READ                4

// Disk label is invalid or corrupted
#define EX_DISK_LABEL               5

// Disk driver could not locate the boot partition
#define EX_DISK_PARTITION           6

// // General filesystem error
// #define EX_FS                       8

// // Filesystem is invalid or corrupted
// #define EX_FS_INVALID               9

// // Filesystem driver could not mount filesystem
// #define EX_FS_MOUNT                 10

// // Loader does not support the boot partition filesystem
// #define EX_FS_UNSUPPORTED           11

// // Filesystem read error
// #define EX_FS_READ                  12

// // Path not found in filesystem
// #define EX_FS_PATH                  13

// // Configuration file not found
// #define EX_CONFIG                   14

// // Syntax error in configuration file
// #define EX_CONFIG_SYNTAX            15


// ============================================================================

#ifndef __ASM__

#ifdef UNIT_TESTING

#define NORETURN

#else

#include <stdnoreturn.h>
#define NORETURN noreturn

#endif // UNIT_TESTING

//
// Macro for raising an exception ex, if err != E_SUCCESS
//
#define ERRCHECK(ex, err) \
    do { \
        if (err != E_SUCCESS) { \
            exceptv(ex, err); \
        } \
    } while (0)


//
// Stop execution of the loader completely. A message is printed to the
// console and the system is halted forever.
//
#define loader_abort() _loader_abort()

//
// Exception occurred, print message relative to the exception ex and abort.
//
#define except(ex) exceptv(ex, 0)

//
// Same as except, but with an additional error code to be displayed
//
NORETURN void exceptv(unsigned ex, int code);

//
// Stop execution of the loader completely. A message is printed and the system
// is halted forever.
//
NORETURN void _loader_abort(void);

#endif  // __ASM__

#endif