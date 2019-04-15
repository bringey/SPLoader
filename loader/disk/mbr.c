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

#ifdef OPT_DISK_MBR

#include <loader/disk/mbr.h>
#include <loader/err.h>
#include <loader/mem.h>

#define MAX_INDEX 4

int ldr_disk_mbr_read(DiskLabel *label) {
    Disk *disk = label->disk;
    // MBR is a measly 512 bytes, just keep it on the stack
    MbrGeneric mbr;
    ldr_disk_read(disk, &mbr, MBR_LBA, MBR_LENGTH, 1);

    // check the signature
    // TODO: make portable (this only works on little-endian platforms)
    if (mbr.signature != MBR_BOOTSIG) {
        return E_MBR_SIGNATURE;
    }

    // read the mbr

    // count the partitions in the read mbr
    size_t nparts = 0;
    for (size_t i = 0; i != 4; ++i) {
        if (mbr.parts[i].type != 0) {
            ++nparts;
        }
    }

    // convert each MbrPart to a DiskPart and store it in the label's table
    label->tablesize = nparts;
    if (nparts) {
        DiskPart *table = (DiskPart*)ldr_malloc(sizeof(DiskPart) * nparts);
        MbrPart *part;
        label->table = table;
        for (size_t i = 0; i != 4; ++i) {
            part = mbr.parts + i;
            if (part->type != 0) {
                table->num = i;
                table->active = (part->status & MBR_STATUS_ACTIVE) == MBR_STATUS_ACTIVE;
                table->startLba = part->firstLba;
                table->endLba = part->firstLba + part->sectors;
                ++table;
            }
        }
    }
    

    return E_SUCCESS;
}

#else

typedef int dummy;

#endif