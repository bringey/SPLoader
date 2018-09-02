/*
** File: src/disk.c
**
** Author: bringey
**
*/

#include <assert.h>
#include <console.h>
#include <disk.h>
#include <err.h>
#include <string.h>



static Disk DISK;


int disk_init(void) {
    return _disk_init(&DISK);
}


int disk_read(uint8_t *buf, uint32_t start, uint32_t blocks) {
    // check overflow
    assert(start <= UINT32_MAX - blocks);
    // make sure we don't read more blocks that exist
    uint32_t limit = start + blocks;
    assert(limit <= DISK.totalBlocks);
    
    int err;
    uint32_t blocksToRead;
    uint32_t transfers, remainder;
    uint32_t bytes;
    if (DISK.maxBlocksPerRead) {
        transfers = blocks / DISK.maxBlocksPerRead;
        remainder = blocks % DISK.maxBlocksPerRead;
    } else {
        transfers = 0;
        remainder = blocks;
    }
    for (size_t i = transfers + 1; i != 0; --i) {
        blocksToRead = (i == 1) ? remainder : transfers;
        err = _disk_read(start, blocksToRead);
        if (err == E_SUCCESS) {
            bytes = blocksToRead * DISK.blocksize;
            memcpy(buf, DISK.buffer, bytes);
            buf += bytes;
        } else {
            break;
        }
    }
    // size_t bytesCopied = 0;

    // while (blocks != 0) {
    //     if (DISK.maxBlocksPerRead) {

    //     }
    //     blocksToRead = blocks > DISK.maxBlocksPerRead ? DISK.maxBlocksPerRead : blocks;
    //     err = _disk_read(start, blocksToRead);
    //     if (err == E_SUCCESS) {
    //         memcpy(buf, DISK.buffer, blocksToRead * DISK.blocksize);
    //         blocks -= blocksToRead;
    //     } else {
    //         break;
    //     }
    // }

    return err;
}

int disk_dump(void) {
    con_printf("[DISK] totalBlocks =  %d\n", DISK.totalBlocks);
    con_printf("[DISK] blocksize = %d\n", DISK.blocksize);
    con_printf("[DISK] maxBlocksPerRead = %d\n", DISK.maxBlocksPerRead);
    con_printf("[DISK] buffer = 0x%08x\n", DISK.buffer);
    return E_SUCCESS;
}