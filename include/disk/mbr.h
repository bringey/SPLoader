/*
** File: include/disk/mbr.h
**
** Author: bringey
**
*/

#ifndef _DISK_MBR_H
#define _DISK_MBR_H

//
// MBR Structure
//

#define MBR_BOOTSTRAP          0x0

#define MBR_PARTITION1       0x1BE
#define MBR_PARTITION2       0x1CE
#define MBR_PARTITION3       0x1DE
#define MBR_PARTITION4       0x1EE

#define MBR_BOOTSIG_LOCATION 0x1FE

//
// MBR Boot Signature
//

#define MBR_BOOTSIG 0xAA55

#define MBR_STATUS_ACTIVE   0x80
#define MBR_STATUS_INACTIVE  0x0

#ifndef __ASM__

#include <stdint.h>

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

//
// Check the MBR for validity. Returns E_FAILURE if the MBR is corrupted
//
int disk_mbr_check(MbrGeneric *mbr);

int disk_mbr_dump(MbrGeneric *mbr);

//
// Find the first bootable partition and store the partition number in numVar
//
int disk_mbr_findBoot(MbrGeneric *mbr, int *numVar);

#endif  // __ASM__


#endif