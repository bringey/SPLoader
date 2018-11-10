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

#include <loader/assert.h>
#include <loader/disk/mbr.h>
#include <loader/err.h>
#include <loader/mem.h>


int disk_mbr_init(DiskLabel *label) {
    Disk *disk = label->disk;
    // allocate a buffer for the MBR
    MbrGeneric *mbr = (MbrGeneric*)mem_malloc(sizeof(MbrGeneric));
    if (mbr == NULL) {
        return E_NOMEM; // should raise EX_MEM
    }
    disk_read(disk, (uint8_t*)mbr, 0, 1);
    // our aux pointer is the MBR we just read from disk
    label->aux = mbr;
    return E_SUCCESS;
}

int disk_mbr_check(DiskLabel *label) {
    assert(label != NULL);
    MbrGeneric *mbr = (MbrGeneric*)label->aux;
    assert(mbr != NULL);

    // check the signature
    if (mbr->signature != MBR_BOOTSIG) {
        return E_MBR_SIGNATURE;
    }

    return E_SUCCESS;
}

int disk_mbr_getActive(DiskLabel *label, DiskPart *part) {
    assert(label != NULL);
    MbrGeneric *mbr = (MbrGeneric*)label->aux;
    assert(mbr != NULL);

    MbrPart *mbrpart = mbr->parts;
    for (size_t i = 0; i != 4; ++i) {
        if (mbrpart->status & MBR_STATUS_ACTIVE) {
            part->num = i;
            part->startLba = mbrpart->firstLba;
            part->endLba = mbrpart->firstLba + mbrpart->sectors;
            return E_SUCCESS;
        }
        ++mbrpart;
    }
    return E_DISK_LABEL_NO_PARTITION;
}

int disk_mbr_getPart(DiskLabel *label, uint32_t index, DiskPart *part) {
    
    if (index >= 4) {
        return E_DISK_LABEL_INDEX;
    }

    assert(label != NULL);
    MbrGeneric *mbr = (MbrGeneric*)label->aux;
    assert(mbr != NULL);

    MbrPart *mbrpart = mbr->parts + index;
    if (mbrpart->type == 0) {
        return E_DISK_LABEL_NO_PARTITION;
    }
    part->num = index;
    part->startLba = mbrpart->firstLba;
    part->endLba = mbrpart->firstLba + mbrpart->sectors;

    return E_FAILURE;
}

#else

typedef int dummy;

#endif