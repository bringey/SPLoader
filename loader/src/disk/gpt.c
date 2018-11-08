/*
** File: src/disk/gpt.c
**
** Author: bringey
**
** NOTE: This file is an optional compile. To use: define OPT_DISK_GPT and add
**       this file to the LOADER_OBJ list.
*/

#ifdef OPT_DISK_GPT

#include <sploader.h>

#include <loader/assert.h>
#include <loader/disk.h>
#include <loader/disk/gpt.h>
#include <loader/err.h>
#include <loader/string.h>


// int disk_gpt_check(void) {

    
//     // 1. Check primary header, if fails try backup
//     // 2. Check partiton table: integrity, validity and no overlapping partitions

//     // GptHeader header;
//     // int err = disk_gpt_checkHeader(GPT_LBA_HEADER, &header);
//     // if (err != E_SUCCESS) {
//     //     // try the backup one
//     //     if (err == E_GPT_HEADER_INTEGRITY) {
//     //         err = disk_gpt_checkHeader(header.alternateLba, &header);
//     //     }
//     // }

//     // if (err == E_SUCCESS) {
//     //     size_t tableSize = header->partitionEntryCount * header->partitionEntrySize;
//     //     stack_alloc(tableSize);
//     // }

//     return E_SUCCESS;


// }


// int disk_gpt_checkHeader(uint64_t lba, GptHeader *header) {
//     assert(header != NULL);

//     // Check procedure, taken from UEFI spec page 124

//     // 1. check signature
//     if (header->signature != GPT_SIGNATURE) {
//         return E_GPT_HEADER_INVALID;
//     }

//     // 2. check the header CRC
//     uint32_t headerCrc = header->headerCrc32;
//     // zero the header crc field (must be zero when calculating the CRC)
//     header->headerCrc32 = 0;
//     uint32_t testCrc = spl_crc32((uint8_t*)header, header->headerSize);
//     // restore the old crc value back in header
//     header->headerCrc32 = headerCrc;
//     if (headerCrc != testCrc) {
//         return E_GPT_HEADER_INTEGRITY;
//     }

//     // 3. check that header->myLba == lba
//     if (header->myLba != lba) {
//         return E_GPT_HEADER_INVALID;
//     }


//     return E_SUCCESS;
// }

int disk_gpt_init(DiskLabel *label) {
    (void)label;
    return E_SUCCESS;
}

int disk_gpt_check(DiskLabel *label) {
    (void)label;
    return E_FAILURE;
}

int disk_gpt_getActive(DiskLabel *label, DiskPart *part) {
    (void)label; (void)part;
    return E_FAILURE;
}

int disk_gpt_getPart(DiskLabel *label, uint32_t index, DiskPart *part) {
    (void)label; (void)index; (void)part;
    return E_FAILURE;
}

#else

typedef int dummy;

#endif