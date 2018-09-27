/*
** File: src/disk.c
**
** Author: bringey
**
*/

#include <abort.h>
#include <assert.h>
#include <console.h>
#include <disk.h>
#include <err.h>
#include <mem.h>
#include <string.h>


DiskLabel disk_detect(Disk *disk) {
    DiskLabel label;
    int err = _disk_detect(disk, &label);
    if (err != E_SUCCESS) {
        exceptv(EX_DISK_LABEL_INVALID, err);
    }

    if (label == DISK_LABEL_UNKNOWN) {
        except(EX_DISK_LABEL_UNKNOWN);
    }

    return label;
}

void disk_bootDisk(Disk *disk) {
    int err = _disk_bootDisk(disk);
    if (err != E_SUCCESS) {
        exceptv(EX_DISK, err);
    }

    disk->blockBuf = (uint8_t*)mem_malloc(disk->blocksize);
}

void disk_bootPart(Disk *disk, DiskLabel label, DiskPart *part) {
    int err = _disk_bootPart(disk, label, part);
    if (err != E_SUCCESS) {
        except(EX_DISK_NO_BOOT);
    }
}


void disk_init(void) {
    int err = _disk_init();
    if (err != E_SUCCESS) {
        exceptv(EX_DISK, err);
    }
}


int disk_read(Disk *disk, uint8_t *buf, uint32_t start, uint32_t blocks) {
    assert(disk != NULL);
    assert(buf != NULL);
    // check overflow
    assert(start <= UINT32_MAX - blocks);
    // make sure we don't read more blocks that exist
    uint32_t limit = start + blocks;
    assert(limit <= disk->totalBlocks);
    
    int err;
    uint32_t blocksToRead;
    uint32_t transfers, remainder;
    uint32_t bytes;
    if (disk->maxBlocksPerRead) {
        transfers = blocks / disk->maxBlocksPerRead;
        remainder = blocks % disk->maxBlocksPerRead;
    } else {
        transfers = 0;
        remainder = blocks;
    }
    for (size_t i = transfers + 1; i != 0; --i) {
        blocksToRead = (i == 1) ? remainder : transfers;
        err = _disk_read(disk, start, blocksToRead);
        if (err == E_SUCCESS) {
            bytes = blocksToRead * disk->blocksize;
            memcpy(buf, disk->buffer, bytes);
            buf += bytes;
        } else {
            except(EX_DISK_READ);
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
