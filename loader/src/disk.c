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

void disk_read(Disk *disk, void *buf, uint64_t lba, uint32_t blocksize, uint32_t blocks) {
    assert(disk != NULL);
    assert(buf != NULL);

    const DiskInfo *info = disk->info;
    assert(info != NULL);

    // overflow? ignore it, assume buf won't be larger than SIZE_MAX
    size_t bytes = blocksize * blocks;
    uint32_t pblocks = bytes / info->blocksize;
    uint32_t leftover = bytes % info->blocksize;

    disk_pread(disk, buf, lba, pblocks);
    // if the logical blocksize was not a multiple of the physical blocksize,
    // read one more block and copy whatever is leftover
    if (leftover) {
        disk_readb(disk, lba + pblocks);
        memcpy((uint8_t*)buf + (bytes - leftover), disk->blockBuf, leftover);
    }


}

void disk_pread(Disk *disk, void *buf, uint64_t start, uint32_t blocks) {
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
    // convert buf to a uint8_t pointer
    uint8_t *bbuf = (uint8_t*)buf;

    // size in blocks of a single transfer (depends on driver)
    uint32_t transferSize = info->maxBlocksPerRead;
    // total number of transfers to do
    uint32_t transfers = blocks / transferSize;
    // size of a transfer in bytes
    uint32_t bytesRead = transfers * info->blocksize;
    // remaining blocks to read
    uint32_t leftovers = blocks % transferSize;

    for (uint32_t i = 0; i != transfers; ++i) {
        err = _disk_read(disk, start, transferSize);
        ERRCHECK(EX_DISK_READ, err);
        memcpy(bbuf, info->buffer, bytesRead);
        start += transferSize;
        bbuf += bytesRead;
    }

    if (leftovers) {
        err = _disk_read(disk, start, leftovers);
        ERRCHECK(EX_DISK_READ, err);
        memcpy(bbuf, info->buffer, leftovers * info->blocksize);
    }
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
    label->maxIndex = 0;
    label->aux = NULL;

    int (*initFn)(DiskLabel*);

    switch (kind) {
        case SPL_DISK_LABEL_UNKNOWN:
        case SPL_DISK_LABEL_NONE:
            exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNKNOWN);
            break;
        case SPL_DISK_LABEL_MBR:
            #ifdef OPT_DISK_MBR
                initFn = disk_mbr_init;
                label->check = disk_mbr_check;
                label->getActive = disk_mbr_getActive;
                label->getPart = disk_mbr_getPart;
            #else
                exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNSUPPORTED);
            #endif
            break;
        case SPL_DISK_LABEL_GPT:
            #ifdef OPT_DISK_GPT
                initFn = disk_gpt_init;
                label->check = disk_gpt_check;
                label->getActive = disk_gpt_getActive;
                label->getPart = disk_gpt_getPart;
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

    // get the total count of partitions
    const uint32_t maxIndex = label->maxIndex;
    size_t nparts = 0;
    for (size_t i = 0; i != maxIndex; ++i) {
        if (label->getPart(label, i, NULL) == E_SUCCESS) {
            ++nparts;
        }
    }

    if (nparts > 0) {
        DiskPart *partbuf = (DiskPart*)mem_malloc(sizeof(DiskPart) * nparts);
        if (partbuf == NULL) {
            except(EX_MEM);
        }

        DiskPart *iter = partbuf;
        for (uint32_t i = 0; i != maxIndex; ++i) {
            if (label->getPart(label, i, iter) == E_SUCCESS) {
                ++iter;
            }
        }

        // test each part for overlap
        DiskPart *left, *right;
        for (size_t i = 0; i != nparts - 1; ++i) {
            left = partbuf + i;
            // test left with all parts ahead of it
            // the parts before it have already been tested
            for (size_t j = i; j != nparts; ++j) {
                right = partbuf + j;
                // test if the left and right partitions overlap
                if (left->endLba > right->startLba || left->startLba < right->endLba) {
                    exceptv(EX_DISK_LABEL, E_DISK_LABEL_OVERLAP);
                }
            }
        }
    }

}
