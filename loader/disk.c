/*
** File: loader/disk.c
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


void ldr_disk_bootDisk(Disk *disk) {
    DiskInfo *info = (DiskInfo*)ldr_malloc(sizeof(DiskInfo));
    int err = _ldr_disk_info(info);
    if (err != E_SUCCESS) {
        ldr_exceptv(EX_DISK, err);
    }
    disk->info = info;
    disk->blockBuf = (uint8_t*)ldr_malloc(info->blocksize);
}

void ldr_disk_init(void) {
    int err = _ldr_disk_init();
    if (err != E_SUCCESS) {
        ldr_exceptv(EX_DISK, err);
    }
}

void ldr_disk_read(Disk *disk, void *buf, uint64_t lba, uint32_t blocksize, uint32_t blocks) {
    ldr_assert(disk != NULL);
    ldr_assert(buf != NULL);

    const DiskInfo *info = disk->info;
    ldr_assert(info != NULL);

    // overflow? ignore it, assume buf won't be larger than SIZE_MAX
    size_t bytes = blocksize * blocks;
    uint32_t pblocks = bytes / info->blocksize;
    uint32_t leftover = bytes % info->blocksize;

    ldr_disk_pread(disk, buf, lba, pblocks);
    // if the logical blocksize was not a multiple of the physical blocksize,
    // read one more block and copy whatever is leftover
    if (leftover) {
        ldr_disk_readb(disk, lba + pblocks);
        ldr_memcpy((uint8_t*)buf + (bytes - leftover), disk->blockBuf, leftover);
    }


}

void ldr_disk_pread(Disk *disk, void *buf, uint64_t start, uint32_t blocks) {
    ldr_assert(disk != NULL);
    ldr_assert(buf != NULL);

    const DiskInfo *info = disk->info;
    ldr_assert(info != NULL);

    // check overflow
    ldr_assert(start <= UINT64_MAX - blocks);
    // make sure we don't read more blocks that exist
    uint64_t limit = start + blocks;
    ldr_assert(limit <= info->totalBlocks);

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
        err = _ldr_disk_read(disk, start, transferSize);
        ERRCHECK(EX_DISK_READ, err);
        ldr_memcpy(bbuf, info->buffer, bytesRead);
        start += transferSize;
        bbuf += bytesRead;
    }

    if (leftovers) {
        err = _ldr_disk_read(disk, start, leftovers);
        ERRCHECK(EX_DISK_READ, err);
        ldr_memcpy(bbuf, info->buffer, leftovers * info->blocksize);
    }
}

void ldr_disk_readb(Disk *disk, uint64_t lba) {
    // convience function, note that disk_read can be used instead. This just
    // makes it easier for reading single blocks at a time
    int err = _ldr_disk_read(disk, lba, 1);
    if (err == E_SUCCESS) {
        ldr_memcpy(disk->blockBuf, disk->info->buffer, disk->info->blocksize);
    } else {
        ldr_exceptv(EX_DISK_READ, err);
    }
}

void ldr_disk_label_init(Disk *disk, SplLabelKind kind, DiskLabel *label) {
    label->disk = disk;
    label->kind = kind;
    label->tablesize = 0;
    label->table = NULL;

    DiskLabelReadFn readFn;

    switch (kind) {
        case SPL_DISK_LABEL_UNKNOWN:
        case SPL_DISK_LABEL_NONE:
            ldr_exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNKNOWN);
            break;
        case SPL_DISK_LABEL_MBR:
            #ifdef OPT_DISK_MBR
                readFn = ldr_disk_mbr_read;
            #else
                ldr_exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNSUPPORTED);
            #endif
            break;
        case SPL_DISK_LABEL_GPT:
            #ifdef OPT_DISK_GPT
                readFn = ldr_disk_gpt_read;
            #else
                ldr_exceptv(EX_DISK_LABEL, E_DISK_LABEL_UNSUPPORTED);
            #endif
            break;
    }

    int err = readFn(label);
    ERRCHECK(EX_DISK_LABEL, err);

    DiskPart *partbuf = label->table;
    size_t nparts = label->tablesize;
    if (nparts > 0) {
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
                    ldr_exceptv(EX_DISK_LABEL, E_DISK_LABEL_OVERLAP);
                }
            }
        }
    }
}

void ldr_disk_label_getActive(DiskLabel *label, DiskPart *part) {
    // scan the table for a DiskPart with active == true
    for (size_t i = 0; i != label->tablesize; ++i) {
        if (label->table[i].active) {
            *part = label->table[i];
            return;
        }
    }
    // no active partition found, abort
    ldr_exceptv(EX_DISK_PARTITION, E_DISK_PARTITION_ACTIVE);
}

void ldr_disk_label_getPart(DiskLabel *label, uint32_t partnum, DiskPart *part) {
    if (partnum >= label->tablesize) {
        ldr_exceptv(EX_DISK_LABEL, E_DISK_LABEL_NO_PARTITION);
    }

    // scan the table for a DiskPart with num == partnum
    size_t nparts = label->tablesize;
    DiskPart *iter = label->table;
    for (size_t i = 0; i != nparts; ++i) {
        if (iter->num == partnum) {
            *part = *iter;
            return;
        }
        ++iter;
    }
    // couldn't find the partition, abort
    ldr_exceptv(EX_DISK_PARTITION, E_DISK_PARTITION_INDEX);
}
