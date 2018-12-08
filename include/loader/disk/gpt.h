/*
** File: include/loader/disk/gpt.h
**
** Author: bringey
**
** NOTE: Optional module, must define OPT_DISK_GPT when including
*/

#ifndef _LOADER_DISK_GPT_H
#define _LOADER_DISK_GPT_H

#ifdef OPT_DISK_GPT

#define GPT_SIGNATURE 0x5452415020494645ULL
#define GPT_VERSION_1 0x10000

// header byte offsets
#define GPT_HEADER_SIGNATURE 0x0
#define GPT_HEADER_REVISION 0x8
#define GPT_HEADER_HEADERSIZE 0xC
#define GPT_HEADER_HEADERCRC 0x10
#define GPT_HEADER_RESERVED 0x14
#define GPT_HEADER_MYLBA 0x18
#define GPT_HEADER_ALTERNATELBA 0x20

// partition attributes
#define GPT_ATTRIBUTE_REQUIRED  0x1     // bit 0
#define GPT_ATTRIBUTE_IGNORED   0x2     // bit 1
#define GPT_ATTRIBUTE_LEGACY    0x4     // bit 2
// bits 3-47 are reserved
// bits 48-63 are partition-specific

#define GPT_ATTRIBUTE_MS_READONLY       0x1000000000000000  // bit 60
#define GPT_ATTRIBUTE_MS_SHADOWCOPY     0x2000000000000000  // bit 61
#define GPT_ATTRIBUTE_MS_HIDDEN         0x4000000000000000  // bit 62
#define GPT_ATTRIBUTE_MS_NOAUTOMOUNT    0x8000000000000000  // bit 63

#define GPT_LBA_HEADER 1
#define GPT_LBA_MBR 0



#ifndef __ASM__

#include <stdint.h>

#include <loader/disk.h>

//
// Structure for a GPT header, field names are inspired by Table 20 from the 
// UEFI Specification version 2.7, page 123
//
struct GptHeader_s {

    uint64_t signature;
    uint32_t revision;
    uint32_t headerSize;
    uint32_t headerCrc32;
    uint32_t reserved;
    uint64_t myLba;
    uint64_t alternateLba;
    uint64_t firstUsableLba;
    uint64_t lastUsuableLba;
    uint8_t diskGuid[16];
    uint64_t partitionEntryLba;
    uint32_t partitionEntryCount;
    uint32_t partitionEntrySize;
    uint32_t partitionEntryCrc32;
    // remainder of the block is reserved and should be zero


} __attribute__((packed));

typedef struct GptHeader_s GptHeader;


struct GptPartitionEntry_s {

    uint8_t partitionTypeGuid[16];
    uint8_t uniquePartitionGuid[16];
    uint64_t startingLba;
    uint64_t endingLba;
    uint64_t attributes;
    uint16_t partitionName[36];

} __attribute__((packed));

typedef struct GptPartitionEntry_s GptPartitionEntry;

int disk_gpt_read(DiskLabel *label);


#endif

#endif

#endif
