/*
** File: include/SPLoader/MBR/MBR.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_MBR_MBR_H
#define _SPLOADER_MBR_MBR_H

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


#endif