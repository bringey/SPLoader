/*
** File: include/loader/disk.h
**
** Author: bringey
*/

#ifndef _LOADER_DISK_H
#define _LOADER_DISK_H

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

//
// Structure containing information about a disk device, as well as driver
// characteristics.
//
typedef struct DiskInfo_s {
    uint64_t totalBlocks;       // Total blocks present on the disk
    uint32_t blocksize;         // Physical size in bytes of a block on disk
    uint32_t maxBlocksPerRead;  // Maximum number of blocks per read
    uint8_t *buffer;            // Transfer buffer location for the driver
    void *aux;                  // auxilary data pointer for the driver
} DiskInfo;

typedef struct Disk_s {

    const DiskInfo *info;       // info struct returned from driver
    uint8_t *blockBuf;          // buffer for a single block transfer

} Disk;

typedef struct DiskPart_s {
    uint8_t num;
    uint64_t startLba;
    uint64_t endLba;
} DiskPart;

//
// Get the boot disk and store it in the given pointer
//
// Exceptions:
//  - EX_DISK: Driver-specific error
//
void disk_bootDisk(Disk *disk);

//
// Finds the boot partition on the boot disk and store it in the part variable.
//
// Exceptions:
//  - EX_DISK_NO_BOOT: The boot partition was not found
//
void disk_bootPart(Disk *disk, DiskLabel label, DiskPart *part);

//
// Detect the disk label on the boot disk. The detected DiskLabel value is
// returned if the disk driver supports it. Note that label of value
// DISK_LABEL_UNKNOWN is never returned, as this is considered an exception
// (EX_DISK_LABEL_INVALID).
//
// Exceptions:
//  - EX_DISK_LABEL_INVALID: Disk label was invalid or corrupted
//  - EX_DISK_LABEL_UNKNOWN: Disk label is unknown to the driver
//
DiskLabel disk_detect(Disk *disk);

//
// Initialize the disk driver for the boot disk. This function must be called
// before any function in this module.
//
// Exceptions:
//  - EX_DISK: Driver-specific error code
//
void disk_init(void);

//
// Read a specified number of blocks starting from an LBA and store the read
// data into buf. If a read error occurs, except will be called and the loader
// will abort.
//
// Exceptions:
//  - EX_DISK_READ: A read error occurred
//
void disk_read(Disk *disk, uint8_t *buf, uint64_t start, uint32_t blocks);

//
// Read a single block from the given disk. The read data is stored in the
// disk's blockBuf member.
//
// Exceptions:
//  - EX_DISK_READ: A read error occurred
//
void disk_readb(Disk *disk, uint64_t lba);


// ============================================================================
// Driver functions

int _disk_bootPart(Disk *disk, DiskLabel label, DiskPart *part);

//
// Detect the disk label if possible. E_SUCCESS is returned on sucessful
// detection. If the label cannot be detected, it is either unknown to
// SPLoader, or is known but is invalid (ie bad CRC in GPT).
//
int _disk_detect(Disk *disk, DiskLabel *label);

//
// Find the boot partition on the boot disk. The partition if found, is stored
// the part variable.
//
int _disk_findBoot(DiskLabel label, DiskPart *part);

int _disk_info(DiskInfo *info);

//
// Initialize system disk driver, a disk structure is given to be set with
// the properties of the driver.
//
int _disk_init(void);

//
// Read from disk. The blocks from start block to start + blocks will be read
// into the internal buffer.
//
int _disk_read(Disk *disk, uint32_t start, uint32_t blocks);




#endif


#endif