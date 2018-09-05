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

// === EXCEPTIONS =============================================================
// Exceptions are fatal errors that are possible throughout the loader code.
// Each exception has a message associated with it to be displayed to the user
// when it is encountered. Some exceptions have an additional error code to be
// displayed as well (ie EX_DISK_LABEL_INVALID)
// ----------------------------------------------------------------------------


// Memory exception
#define EX_MEM                      0

// General disk exception
#define EX_DISK                     1

// Read error in disk
#define EX_DISK_READ                2

// Disk label is invalid or corrupted
#define EX_DISK_LABEL_INVALID       3

// Disk label is not supported by the disk driver
#define EX_DISK_LABEL_UNSUPPORTED   4

// Disk driver could not locate the boot partition
#define EX_DISK_NO_BOOT             5

// General filesystem error
#define EX_FS                       6

// Filesystem is invalid or corrupted
#define EX_FS_INVALID               7

// Filesystem driver could not mount filesystem
#define EX_FS_MOUNT                 8

// Loader does not support the boot partition filesystem
#define EX_FS_UNSUPPORTED           9

// Filesystem read error
#define EX_FS_READ                  10

// Path not found in filesystem
#define EX_FS_PATH                  11

// Configuration file not found
#define EX_CONFIG                   12

// Syntax error in configuration file
#define EX_CONFIG_SYNTAX            13

// Assertion has failed (see assert.h)
#define EX_ASSERT                   14

// ============================================================================

#ifndef __ASM__

#include <stdnoreturn.h>
#include <stdbool.h>


//
// Stop execution of the loader completely. A message is printed to the
// console and the system is halted forever.
//
#define abort() _abort()

//
// Exception occurred, print message relative to the exception ex and abort.
//
#define except(ex) except_(ex, 0, false)

//
// Same as except, but with an additional error code to be displayed
//
#define exceptv(ex, code) except_(ex, code, true)


noreturn void except_(unsigned ex, int code, bool hasCode);

//
// Stop execution of the loader completely. A message is printed and the system
// is halted forever.
//
noreturn void _abort(void);

#endif  // __ASM__

#endif