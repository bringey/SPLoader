/*
** File: libsploader/label/mbr.c
**
** Author: bringey
**
** Contains the read function for MBR disk labels
*/

#include <sploader.h>

#ifdef SPL_OPT_MBR

//
// MBR Structure
//

#define MBR_BOOTSTRAP          0x0

#define MBR_PARTITION1       0x1BE
#define MBR_PARTITION2       0x1CE
#define MBR_PARTITION3       0x1DE
#define MBR_PARTITION4       0x1EE

#define MBR_BOOTSIG_LOCATION 0x1FE

#define MBR_LENGTH  512
#define MBR_LBA     0

//
// MBR Boot Signature
//

#define MBR_BOOTSIG 0xAA55
#define MBR_BOOTSIG_LO 0x55
#define MBR_BOOTSIG_HI 0xAA

#define MBR_STATUS_ACTIVE   0x80
#define MBR_STATUS_INACTIVE  0x0


struct MbrPart_s {
    uint8_t status;         // 0x80 = boot 0x0 = inactive
    uint8_t firstChs[3];    // CHS address of the first absolute sector
    uint8_t type;           // type of the partition 
    uint8_t lastChs[3];     // CHS address of the last absolute sector
    uint32_t firstLba;      // LBA address of the first sector
    uint32_t sectors;       // number of sectors in the partition
} __attribute__((packed));

typedef struct MbrPart_s MbrPart;

struct MbrGeneric_s {

    uint8_t bootstrap[446]; // bootstrap code area
    MbrPart parts[4];       // partition table
    uint16_t signature;     // Boot signature 0xAA55

} __attribute__((packed));

typedef struct MbrGeneric_s MbrGeneric;


int spl_label_mbr_read(SplLabel *label) {

    int err;
    SplDev *dev = label->dev;
    err = spl_dev_read(dev, dev->bbuf, MBR_LBA, 1);
    if (err != SPL_E_SUCCESS) {
        return err;
    }

    MbrGeneric *mbr = (MbrGeneric*)dev->bbuf.loc;

    // check the signature
    uint8_t *sig = (uint8_t*)&mbr->signature;
    if (sig[0] != MBR_BOOTSIG_LO || sig[1] != MBR_BOOTSIG_HI) {
        return SPL_E_MBR_SIGNATURE;
    }

    // read the mbr

    // EXTRA: add EBR support
    size_t nparts = 0;
    MbrPart *srcpart = mbr->parts;
    SplPart *dstpart = label->table;
    for (size_t i = 0; i != 4; ++i) {
        if (nparts >= label->tablesize) {
            break;
        }

        if (srcpart->type != 0) {
            dstpart->num = i;
            dstpart->active = (srcpart->status & MBR_STATUS_ACTIVE) == MBR_STATUS_ACTIVE;
            dstpart->startLba = srcpart->firstLba;
            dstpart->endLba = srcpart->firstLba + srcpart->sectors;
            ++srcpart;
            ++dstpart;
            ++nparts;
        }
    }

    label->nparts = nparts;

    return SPL_E_SUCCESS;
}

#else

typedef int dummy;

#endif