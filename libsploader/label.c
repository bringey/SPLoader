/*
** File: libsploader/label.c
**
** Author: bringey
**
** Functions for reading a disk label from a SplDev
*/

#include <sploader.h>


int spl_label_getActive(SplLabel *label, SplPart *part) {
    // scan the table for a DiskPart with active == true
    SplPart *p = label->table;
    for (size_t i = 0; i != label->tablesize; ++i, ++p) {
        if (p->active) {
            *part = *p;
            return SPL_E_SUCCESS;
        }
    }

    // no active partition found, return failure
    return SPL_E_FAILURE;
}

int spl_label_getPart(SplLabel *label, uint32_t partnum, SplPart *part) {

    // scan the table for a SplPart with num == partnum
    size_t tablesize = label->tablesize;
    SplPart *iter = label->table;
    for (size_t i = 0; i != tablesize; ++i) {
        if (iter->num == partnum) {
            *part = *iter;
            return SPL_E_SUCCESS;
        }
        ++iter;
    }
    // couldn't find the partition
    return SPL_E_FAILURE;

}

int spl_label_read(SplLabel *label) {
    int (*readFn)(SplLabel *label) = NULL;

    switch (label->kind) {
        case SPL_DISK_LABEL_UNKNOWN:
            break;
        case SPL_DISK_LABEL_NONE:
            // no disk label, do nothing
            return SPL_E_SUCCESS;
        case SPL_DISK_LABEL_MBR:
            #ifdef SPL_OPT_MBR
                readFn = spl_label_mbr_read;
            #endif
            break;
        case SPL_DISK_LABEL_GPT:
            #ifdef SPL_OPT_GPT
                readFn = spl_label_gpt_read;
            #endif
            break;
    }

    if (readFn == NULL) {
        return SPL_E_LABEL_UNKNOWN;
    }

    return readFn(label);
}

int spl_label_valid(SplLabel *label) {
    size_t tablesize = label->tablesize;
    SplPart *table = label->table;
    uint32_t maxlba = label->dev->totalBlocks;
    if (tablesize > 0) {

        // bounds check each part
        SplPart *p = table;
        for (size_t i = 0; i != tablesize; ++i, ++p) {
            if (p->startLba > p->endLba || p->endLba >= maxlba) {
                // a partition's ending lba is less than the starting lba OR
                // the starting/ending lba is greater than or equal to the
                // maximum lba on the device (dev->totalBlocks)
                return SPL_E_LABEL_BOUNDS;
            }
        }

        // test each part for overlap
        SplPart *left, *right;
        for (size_t i = 0; i != tablesize - 1; ++i) {
            left = table + i;
            // test left with all parts ahead of it
            // the parts before it have already been tested
            for (size_t j = i + 1; j != tablesize; ++j) {
                right = table + j;
                // test if the left and right partitions overlap
                // left and right overlap if
                // w = left->startLba, x = right->endLba, y = left->endLba, z = right->startLba
                // !((w >= x) || (y <= z))
                if (!(left->startLba >= right->endLba || left->endLba <= right->startLba)) {
                    return SPL_E_LABEL_OVERLAP;
                }
            }
        }
    }

    return SPL_E_SUCCESS;

}
