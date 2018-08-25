/*
** File: include/disk.h
**
** Author: bringey
*/

#ifndef _LOADER_DISK_H
#define _LOADER_DISK_H

#ifndef __ASM__

#include <stdint.h>

#define disk_init() _disk_init()
#define disk_blockSize() _disk_blockSize()
#define disk_read(start, blocks) _disk_read(start, blocks)
#define disk_buffer() _disk_buffer()

//
// Initialize basic disk driver
//
int _disk_init(void);

//
// Read from disk. The blocks from start block to start + blocks will be read
// into buffer.
//
int _disk_read(uint32_t start, uint32_t blocks);

//
// Gets the block size (in bytes) of the disk driver
//
uint32_t _disk_blockSize(void);

uint8_t* _disk_buffer(void);

#endif


#endif