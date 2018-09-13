/*
** File: include/disk.h
**
** Author: bringey
*/

#ifndef _DISK_H
#define _DISK_H

#ifndef __ASM__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef enum {

    DISK_LABEL_UNKNOWN   = 0x0,   // Disk label is unknown or corrupted
    DISK_LABEL_NONE      = 0x1,   // No label detected on disk (for embedded installs)
    DISK_LABEL_MBR       = 0x2,   // MBR
    DISK_LABEL_GPT       = 0x4,   // GPT + Protective MBR
    
} DiskLabel;

typedef struct Disk_s {

    uint32_t supportedLabels;  // bitmap of labels supported by the driver
    uint32_t totalBlocks;
    uint32_t blocksize;
    uint32_t maxBlocksPerRead;
    uint8_t *buffer;

} Disk;

typedef struct DiskPart_s {
    uint8_t num;
    uint64_t startLba;
    uint64_t endLba;
} DiskPart;

//
// Detect the disk label on the boot disk. The detected DiskLabel value is
// returned if the disk driver supports it. Note that label of value
// DISK_LABEL_UNKNOWN is never returned, as this is considered an exception
// (EX_DISK_LABEL_INVALID).
//
// Exceptions:
//  - EX_DISK_LABEL_INVALID: Disk label was unknown or corrupted
//  - EX_DISK_LABEL_UNSUPPORTED: Disk label is not supported by the driver
//
DiskLabel disk_detect(void);

//
// Finds the boot partition on the boot disk and store it in the part variable.
//
// Exceptions:
//  - EX_DISK_NO_BOOT: The boot partition was not found
//
void disk_findBoot(DiskLabel label, DiskPart *part);

//
// Initialize the disk driver for the boot disk. This function must be called
// before any function in this module.
//
// Exceptions:
//  - EX_DISK: Driver-specific error code
//
int disk_init(Disk *disk);

//
// Read a specified number of blocks starting from an LBA and store the read
// data into buf. If a read error occurs, except will be called and the loader
// will abort.
//
// Exceptions:
//  - EX_DISK_READ: A read error occurred
//
int disk_read(uint8_t *buf, uint32_t start, uint32_t blocks);

int disk_dump(void);

// ============================================================================
// Driver functions

//
// Detect the disk label if possible. E_SUCCESS is returned on sucessful
// detection. If the label cannot be detected, it is either unknown to
// SPLoader, or is known but is invalid (ie bad CRC in GPT).
//
int _disk_detect(DiskLabel *label);

//
// Find the boot partition on the boot disk. The partition if found, is stored
// the part variable.
//
int _disk_findBoot(DiskLabel label, DiskPart *part);

//
// Initialize system disk driver, a disk structure is given to be set with
// the properties of the driver.
//
int _disk_init(Disk *disk);

//
// Read from disk. The blocks from start block to start + blocks will be read
// into the internal buffer.
//
int _disk_read(uint32_t start, uint32_t blocks);




#endif


#endif