/*
** File: src/disk/mbr.c
**
** Author: bringey
**
** Contains function implementations for the disk/mbr.h module.
**
** NOTE: This file is an optional compile. To use: define OPT_DISK_MBR and add
**       this file to the LOADER_OBJ list (in platform makefile or includes).
**
*/

#ifndef OPT_DISK_MBR
#define OPT_DISK_MBR
#endif

#include <blocklist.h>
#include <disk/mbr.h>
#include <err.h>
#include <console.h>


int disk_mbr_check(MbrGeneric *mbr) {

    // check the signature
    if (mbr->signature != MBR_BOOTSIG) {
        return E_FAILURE;
    }

    // check for overlapping partitions
    Block32 blocklist[4];
    Blocklist list = {
        .blocks = blocklist,
        .len = 0,
        .capacity = 4
    };
    Block32 block = {0};
    MbrPart *part;
    for (int i = 0; i != 4; ++i) {
        part = mbr->parts + i;
        // skip the partition if its type is 0 (type 0 = not used)
        if (part->type != 0) {
            block.base = part->firstLba;
            block.limit = part->firstLba + part->sectors;
            if (blocklist_insert(&list, block, false) != E_SUCCESS) {
                return E_FAILURE;
            }
        }
    }

    return E_SUCCESS;
}

int disk_mbr_dump(MbrGeneric *mbr) {
    con_printf("Num   Type   LBA Start   LBA end\n");
    for (int i = 0; i != 4; ++i) {
        MbrPart part = mbr->parts[i];
        con_printf("%3d   %4d   %9d   %9d\n", i, part.type, part.firstLba, part.firstLba + part.sectors);
    }
    con_printf("Signature: 0x%04x\n", mbr->signature);
    return E_SUCCESS;
}

int disk_mbr_findBoot(MbrGeneric *mbr, int *numVar) {

    MbrPart *part;
    for (int i = 0; i != 4; ++i) {
        part = mbr->parts + i;
        if ((part->status & MBR_STATUS_ACTIVE) == MBR_STATUS_ACTIVE) {
            *numVar = i;
            return E_SUCCESS;
        }

    }

    return E_FAILURE;
}