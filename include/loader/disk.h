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

#include <sploader.h>


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

typedef struct DiskLabel_s DiskLabel;
typedef struct DiskPart_s DiskPart;

struct DiskLabel_s {
    Disk *disk;                 // handle to disk
    SplDiskLabel kind;          // Label type
    size_t maxIndex;            // maximum number of partitions
    void *aux;                  // label-specific data pointer
    int (*check)(DiskLabel*);
    int (*getActive)(DiskLabel*, uint32_t*);
    int (*getPart)(DiskLabel*, uint32_t, DiskPart*);
};

struct DiskPart_s {
    uint32_t num;
    bool active;
    uint64_t startLba;
    uint64_t endLba;
};

//
// Get the boot disk and store it in the given pointer
//
// Exceptions:
//  - EX_DISK: Driver-specific error
//
void disk_bootDisk(Disk *disk);

//
// Initialize the disk driver for the boot disk. This function must be called
// before any function in this module.
//
// Exceptions:
//  - EX_DISK: Driver-specific error code
//
void disk_init(void);

//
// Logical disk read. Similar to disk_pread, except uses a provided blocksize.
//
void disk_read(Disk *disk, void *buf, uint64_t lba, uint32_t blocksize, uint32_t blocks);

//
// Physical disk read. Read a specified number of blocks starting from an LBA
// and store the read data into buf. The size of a block depends on the disk's
// physical block size (disk->info->blocksize). If a read error occurs, except
// will be called and the loader will abort.
//
// Exceptions:
//  - EX_DISK_READ: A read error occurred
//
void disk_pread(Disk *disk, void *buf, uint64_t start, uint32_t blocks);

//
// Read a single block from the given disk. The read data is stored in the
// disk's blockBuf member.
//
// Exceptions:
//  - EX_DISK_READ: A read error occurred
//
void disk_readb(Disk *disk, uint64_t lba);

void disk_label_init(Disk *disk, SplDiskLabel kind, DiskLabel *label);

void disk_label_check(DiskLabel *label);


// ============================================================================
// Driver functions

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