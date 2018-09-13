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


static Disk BOOT_DISK;


DiskLabel disk_detect(void) {
    DiskLabel label;
    int err = _disk_detect(&label);
    if (err != E_SUCCESS) {
        exceptv(EX_DISK_LABEL_INVALID, err);
    }

    if ((label & BOOT_DISK.supportedLabels) == 0) {
        except(EX_DISK_LABEL_UNSUPPORTED);
    }

    return label;
}

void disk_findBoot(DiskLabel label, DiskPart *part) {
    int err = _disk_findBoot(label, part);
    if (err != E_SUCCESS) {
        except(EX_DISK_NO_BOOT);
    }
}


int disk_init(Disk *disk) {
    int err = _disk_init(&BOOT_DISK);
    if (err != E_SUCCESS) {
        exceptv(EX_DISK, err);
    }
    *disk = BOOT_DISK;
    return err;
}


int disk_read(uint8_t *buf, uint32_t start, uint32_t blocks) {
    // check overflow
    assert(start <= UINT32_MAX - blocks);
    // make sure we don't read more blocks that exist
    uint32_t limit = start + blocks;
    assert(limit <= BOOT_DISK.totalBlocks);
    
    int err;
    uint32_t blocksToRead;
    uint32_t transfers, remainder;
    uint32_t bytes;
    if (BOOT_DISK.maxBlocksPerRead) {
        transfers = blocks / BOOT_DISK.maxBlocksPerRead;
        remainder = blocks % BOOT_DISK.maxBlocksPerRead;
    } else {
        transfers = 0;
        remainder = blocks;
    }
    for (size_t i = transfers + 1; i != 0; --i) {
        blocksToRead = (i == 1) ? remainder : transfers;
        err = _disk_read(start, blocksToRead);
        if (err == E_SUCCESS) {
            bytes = blocksToRead * BOOT_DISK.blocksize;
            memcpy(buf, BOOT_DISK.buffer, bytes);
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

int disk_dump(void) {
    con_printf("[DISK] totalBlocks =  %d\n", BOOT_DISK.totalBlocks);
    con_printf("[DISK] blocksize = %d\n", BOOT_DISK.blocksize);
    con_printf("[DISK] maxBlocksPerRead = %d\n", BOOT_DISK.maxBlocksPerRead);
    con_printf("[DISK] buffer = 0x%08x\n", BOOT_DISK.buffer);
    return E_SUCCESS;
}