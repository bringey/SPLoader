/*
** File: src/disk.c
**
** Author: bringey
**
*/

#include <loader/abort.h>
#include <loader/assert.h>
#include <loader/console.h>
#include <loader/disk.h>
#include <loader/err.h>
#include <loader/mem.h>
#include <loader/string.h>
#include <loader/disk/mbr.h>
#include <loader/disk/gpt.h>


void disk_bootDisk(Disk *disk) {
    DiskInfo *info = (DiskInfo*)mem_malloc(sizeof(DiskInfo));
    int err = _disk_info(info);
    if (err != E_SUCCESS) {
        exceptv(EX_DISK, err);
    }
    disk->info = info;
    disk->blockBuf = (uint8_t*)mem_malloc(info->blocksize);
}

void disk_init(void) {
    int err = _disk_init();
    if (err != E_SUCCESS) {
        exceptv(EX_DISK, err);
    }
}


void disk_read(Disk *disk, uint8_t *buf, uint64_t start, uint32_t blocks) {
    assert(disk != NULL);
    assert(buf != NULL);

    const DiskInfo *info = disk->info;
    assert(info != NULL);

    // check overflow
    assert(start <= UINT64_MAX - blocks);
    // make sure we don't read more blocks that exist
    uint64_t limit = start + blocks;
    assert(limit <= info->totalBlocks);

    int err;
    // size in blocks of a single transfer
    uint32_t transferSize = info->maxBlocksPerRead;
    // blocks to read for a transfer: min(transferSize, blocks)
    uint32_t blocksToRead;
    // bytes to copy to buf
    uint32_t bytes;

    while (start != limit) {
        if (transferSize != 0 && blocks > transferSize) {
            blocksToRead = transferSize;
        } else {
            blocksToRead = blocks;
        }
        err = _disk_read(disk, start, blocksToRead);
        if (err == E_SUCCESS) {
            bytes = blocksToRead * info->blocksize;
            memcpy(buf, info->buffer, bytes);
            buf += bytes;
            start += blocksToRead;
            blocks -= blocksToRead;
        } else {
            exceptv(EX_DISK_READ, err);
        }
    }

    // int err;
    // // size in blocks for a single transfer
    // uint32_t transferSize = disk->maxBlocksPerRead;
    // // number of blocks to read for the iteration
    // uint32_t blocksToRead;
    // // number of transfers to do
    // uint32_t transfers, final;
    // uint32_t bytes;
    // if (transferSize) {
    //     transfers = blocks / disk->maxBlocksPerRead;
    //     remainder = blocks % disk->maxBlocksPerRead;
    // } else {
    //     transfers = 0;
    //     remainder = blocks;
    // }
    // for (size_t i = transfers + 1; i != 0; --i) {
    //     blocksToRead = (i == 1) ? remainder : disk;
    //     err = _disk_read(disk, start, blocksToRead);
    //     if (err == E_SUCCESS) {
    //         bytes = blocksToRead * disk->blocksize;
    //         memcpy(buf, disk->buffer, bytes);
    //         buf += bytes;
    //     } else {
    //         except(EX_DISK_READ);
    //     }
    // }
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

}

void disk_readb(Disk *disk, uint64_t lba) {
    // convience function, note that disk_read can be used instead. This just
    // makes it easier for reading single blocks at a time
    int err = _disk_read(disk, lba, 1);
    if (err == E_SUCCESS) {
        memcpy(disk->blockBuf, disk->info->buffer, disk->info->blocksize);
    } else {
        exceptv(EX_DISK_READ, err);
    }
}

void disk_label_init(Disk *disk, SplDiskLabel kind, DiskLabel *label) {
    label->disk = disk;
    label->kind = kind;
    int (*initFn)(DiskLabel*);
    switch (kind) {
        case SPL_DISK_LABEL_UNKNOWN:
        case SPL_DISK_LABEL_NONE:
            exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNKNOWN);
            break;
        case SPL_DISK_LABEL_MBR:
            #ifdef OPT_DISK_MBR
                label->check = disk_mbr_check;
                label->getActive = disk_mbr_getActive;
                label->getPart = disk_mbr_getPart;
                initFn = disk_mbr_init;
            #else
                exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNSUPPORTED);
            #endif
            break;
        case SPL_DISK_LABEL_GPT:
            #ifdef OPT_DISK_GPT
                disk_gpt_init(label);
                label->check = disk_gpt_check;
                label->getActive = disk_mbr_getActive;
                label->getPart = disk_mbr_getPart;
                initFn = disk_gpt_init;
            #else
                exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNSUPPORTED);
            #endif
            break;
    }

    int err = initFn(label);
    ERRCHECK(EX_DISK_LABEL, err);
}


void disk_label_check(DiskLabel *label) {
    int err = label->check(label);
    ERRCHECK(EX_DISK_LABEL, err);
}

void disk_label_getActive(DiskLabel *label, DiskPart *part) {
    int err = label->getActive(label, part);
    ERRCHECK(EX_DISK_LABEL, err);
}

void disk_label_getPart(DiskLabel *label, uint32_t index, DiskPart *part) {
    int err = label->getPart(label, index, part);
    ERRCHECK(EX_DISK_LABEL, err);
}
