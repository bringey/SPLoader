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
#define E_NONE E_SUCCESS

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
// EX_HEADER
// ============================================================================

#define E_HEADER_INVALID 1

#define E_HEADER_INTEGRITY 2

#define E_HEADER_LOADER_INTEGRITY 3

// ============================================================================
// EX_DISK_LABEL
// ============================================================================

#define E_DISK_LABEL_UNKNOWN                1
#define E_DISK_LABEL_UNSUPPORTED            2
// partition index is invalid
#define E_DISK_LABEL_INDEX                  3
// partition index is not a valid partition
#define E_DISK_LABEL_NO_PARTITION           4
// overlapping partitions
#define E_DISK_LABEL_OVERLAP                5

//
// MBR exception codes (100-199)
//

// MBR contains an invalid signature (signature != 0xAA55)
#define E_MBR_SIGNATURE                     100

// GPT Exception codes (200-299)

// unsupported version
#define E_GPT_VERSION                       200
// incorrect crc32 checksum for header
#define E_GPT_HEADER_INTEGRITY              201
// incorrect crc32 checksum for partition table
#define E_GPT_PARTITION_INTEGRITY           202
// invalid header
#define E_GPT_HEADER_INVALID                203
// partition entry invalid
#define E_GPT_PARTITION_INVALID             204

// ============================================================================
// EX_DISK_PARTITION
// ============================================================================

// parition does not exist in label
#define E_DISK_PARTITION_INDEX              1
// active partition was not found
#define E_DISK_PARTITION_ACTIVE             2


#endif 