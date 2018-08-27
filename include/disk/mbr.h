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

#ifndef __ASM__

#include <stdint.h>

typedef struct MbrPart_s {
    uint8_t status;         // 0x80 = boot 0x0 = inactive
    uint8_t firstChs[3];    // CHS address of the first absolute sector
    uint8_t type;           // type of the partition 
    uint8_t lastChs[3];     // CHS address of the last absolute sector
    uint32_t firstLba;      // LBA address of the first sector
    uint32_t sectors;       // number of sectors in the partition
} MbrPart;

typedef struct MbrGeneric_s {

    uint8_t bootstrap[446]; // bootstrap code area
    MbrPart parts[4];       // partition table
    uint16_t signature;     // Boot signature 0xAA55

} MbrGeneric;


#endif  // __ASM__


#endif