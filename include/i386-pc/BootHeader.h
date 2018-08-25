/*
** File: include/i386-pc/BootHeader.h
**
** Author: bringey
*/

#ifndef _I386_PC_BOOTHEADER_H
#define _I386_PC_BOOTHEADER_H

#define BOOT_HEADER_SIGNATURE_HI 0x52454441   // "ADER" little-endian
#define BOOT_HEADER_SIGNATURE_LO 0x4F4C5053   // "SPLO" little-endian

#define BOOT_HEADER_SIGNATURE   0x0
#define BOOT_HEADER_FLAGS       0x8
#define BOOT_HEADER_LOADER_SIZE 0xC

#ifndef __ASM__

#include <stdint.h>

//
// Structure definition for the i386-pc boot header. The boot header is located
// in the first sector or 512 bytes of the boot partition. For BIOS/MBR setups,
// this partition is located in the post-MBR gap or LBA 1. For BIOS/GPT setups,
// this partition is located in a GPT partition with type "BIOS Boot partition".
//
// Structure of the boot partition:
//
// LBA offset
//     +0 +---------------+
//        | header        |
//     +1 +---------------+
//        | loader.bin    |
//        | (size varies) |
//        |               |
//        +---------------+
//
//
struct BootHeader_s {
    uint64_t signature;
    uint32_t flags;
    uint32_t loaderSize;   // size of loader.bin in bytes
    uint8_t reserved[496]; // reserved for future use

} __attribute__((packed));

typedef struct BootHeader_s BootHeader;


#endif  // __ASM__

#endif