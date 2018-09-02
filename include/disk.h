/*
** File: include/disk.h
**
** Author: bringey
*/

#ifndef _DISK_H
#define _DISK_H

#ifndef __ASM__

#include <stddef.h>
#include <stdint.h>

typedef struct Disk_s {

    uint32_t totalBlocks;
    uint32_t blocksize;
    uint32_t maxBlocksPerRead;
    uint8_t *buffer;

} Disk;


int disk_init(void);

int disk_read(uint8_t *buf, uint32_t start, uint32_t blocks);

int disk_dump(void);

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