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

#define MAX_INDEX 4


int disk_mbr_init(DiskLabel *label) {
    Disk *disk = label->disk;
    // allocate a buffer for the MBR
    MbrGeneric *mbr = (MbrGeneric*)mem_malloc(sizeof(MbrGeneric));
    if (mbr == NULL) {
        return E_NOMEM; // should raise EX_MEM
    }
    disk_read(disk, mbr, MBR_LBA, MBR_LENGTH, 1);
    // mbr has 4 partition entries so getPart will only work for indices 0-3
    label->maxIndex = MAX_INDEX;
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

int disk_mbr_getActive(DiskLabel *label, uint32_t *index) {
    assert(label != NULL);
    MbrGeneric *mbr = (MbrGeneric*)label->aux;
    assert(mbr != NULL);

    MbrPart *mbrpart = mbr->parts;
    for (size_t i = 0; i != MAX_INDEX; ++i) {
        if (mbrpart->status & MBR_STATUS_ACTIVE) {
            *index = i;
            return E_SUCCESS;
        }
        ++mbrpart;
    }
    return E_FAILURE;
}

int disk_mbr_getPart(DiskLabel *label, uint32_t index, DiskPart *part) {
    
    if (index >= MAX_INDEX) {
        return E_DISK_LABEL_INDEX;
    }

    assert(label != NULL);
    MbrGeneric *mbr = (MbrGeneric*)label->aux;
    assert(mbr != NULL);

    MbrPart *mbrpart = mbr->parts + index;
    if (mbrpart->type == 0) {
        return E_DISK_LABEL_NO_PARTITION;
    }

    if (part != NULL) {
        part->num = index;
        part->startLba = mbrpart->firstLba;
        part->endLba = mbrpart->firstLba + mbrpart->sectors;
    }

    return E_SUCCESS;
}

#else

typedef int dummy;

#endif