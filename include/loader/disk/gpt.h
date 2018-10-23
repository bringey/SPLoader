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

#define GPT_LBA_HEADER 1
#define GPT_LBA_MBR 0



#ifndef __ASM__

#include <stdint.h>


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

//
// Parse the GPT header block and store it in the given header variable.
// If isBackup is true, then
//
int disk_gpt_check(void);

int disk_gpt_header(GptHeader *header);

int disk_gpt_tablesize(GptHeader *header);

int disk_gpt_table(GptPartitionEntry *buf);


int disk_gpt_checkHeader(uint64_t lba, GptHeader *header);

int disk_gpt_checkTable(GptHeader *header, GptPartitionEntry *table);

//
// Calculate a CRC32 checksum for the given data buffer. 
//
uint32_t disk_gpt_crc32(uint8_t *data, size_t size);

#endif

#endif

#endif
