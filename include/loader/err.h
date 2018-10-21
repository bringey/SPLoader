/*
** File: include/loader/err.h
**
** Author: bringey
**
** Contains macros for error codes used throughout the loader source.
**
*/

#ifndef _LOADER_ERR_H
#define _LOADER_ERR_H


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
#define E_NOMEM 5

//
// Disk Errors (100-200)
//

// generic read error
#define E_DISK_READ 100

// ============================================================================
// EX_DISK_LABEL_INVALID
// ============================================================================

//
// MBR exception codes (0-100)
//

// MBR contains an invalid signature (signature != 0xAA55)
#define E_MBR_SIGNATURE                     0
// MBR partitions overlap
#define E_MBR_PARTITION_OVERLAP             1

// GPT Exception codes (100-200)

// unsupported version
#define E_GPT_VERSION                       100
// incorrect crc32 checksum for header
#define E_GPT_HEADER_INTEGRITY              101
// incorrect crc32 checksum for partition table
#define E_GPT_PARTITION_INTEGRITY           102
// invalid header
#define E_GPT_HEADER_INVALID                103
// partition entry invalid
#define E_GPT_PARTITION_INVALID             104
// overlapping partitions
#define E_GPT_PARTITION_OVERLAP             105

#endif 