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

    DISK_LABEL_CORRUPTED = 0x0,   // Disk label is unknown or corrupted
    DISK_LABEL_NONE      = 0x1,   // No label detected on disk (for embedded installs)
    DISK_LABEL_MBR       = 0x2,   // MBR
    DISK_LABEL_GPT       = 0x4,   // GPT + Protective MBR
    
} DiskLabel;

typedef struct Disk_s {

    DiskLabel label;
    uint32_t supportedLabels;  // bitmap of labels supported by the driver
    uint32_t totalBlocks;
    uint32_t blocksize;
    uint32_t maxBlocksPerRead;
    uint8_t *buffer;

} Disk;

// typedef struct DiskPart_s {
//     uint8_t num;
//     Block32 block;
// } DiskPart;

extern Disk BOOT_DISK;
//extern DiskPart BOOT_PART;


int disk_init(void);

int disk_read(uint8_t *buf, uint32_t start, uint32_t blocks);

int disk_dump(void);

//int _disk_findBoot(DiskLabel label, DiskPart *part);

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