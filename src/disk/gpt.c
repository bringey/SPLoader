/*
** File: src/disk/gpt.c
**
** Author: bringey
**
** NOTE: This file is an optional compile. To use: define OPT_DISK_GPT and add
**       this file to the LOADER_OBJ list.
*/


#include <assert.h>
#include <disk.h>
#include <disk/gpt.h>
#include <err.h>
#include <string.h>

static uint32_t reverse(uint32_t num);


int disk_gpt_check(void) {

    
    // 1. Check primary header, if fails try backup
    // 2. Check partiton table: integrity, validity and no overlapping partitions

    // GptHeader header;
    // int err = disk_gpt_checkHeader(GPT_LBA_HEADER, &header);
    // if (err != E_SUCCESS) {
    //     // try the backup one
    //     if (err == E_GPT_HEADER_INTEGRITY) {
    //         err = disk_gpt_checkHeader(header.alternateLba, &header);
    //     }
    // }

    // if (err == E_SUCCESS) {
    //     size_t tableSize = header->partitionEntryCount * header->partitionEntrySize;
    //     stack_alloc(tableSize);
    // }

    return E_SUCCESS;


}


int disk_gpt_checkHeader(uint64_t lba, GptHeader *header) {
    assert(header != NULL);

    // Check procedure, taken from UEFI spec page 124

    // 1. check signature
    if (header->signature != GPT_SIGNATURE) {
        return E_GPT_HEADER_INVALID;
    }

    // 2. check the header CRC
    uint32_t headerCrc = header->headerCrc32;
    // zero the header crc field (must be zero when calculating the CRC)
    header->headerCrc32 = 0;
    uint32_t testCrc = disk_gpt_crc32((uint8_t*)header, header->headerSize);
    // restore the old crc value back in header
    header->headerCrc32 = headerCrc;
    if (headerCrc != testCrc) {
        return E_GPT_HEADER_INTEGRITY;
    }

    // 3. check that header->myLba == lba
    if (header->myLba != lba) {
        return E_GPT_HEADER_INVALID;
    }


    return E_SUCCESS;
}

uint32_t disk_gpt_crc32(uint8_t *data, size_t size) {

    uint32_t byte;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i != size; ++i) {
        byte = reverse(data[i]);
        for (size_t j = 0; j != 8; ++j) {
            if ((int)(crc ^ byte) < 0) {
                crc = (crc << 1) ^ 0x04C11DB7;
            } else {
                crc <<= 1;
            }
            byte <<= 1;
        }
    }

    return reverse(~crc);

}

uint32_t reverse(uint32_t num) {
    num = ((num & 0x55555555) << 1) | ((num >> 1) & 0x55555555);
    num = ((num & 0x33333333) << 2) | ((num >> 2) & 0x33333333);
    num = ((num & 0x0F0F0F0F) << 4) | ((num >> 4) & 0x0F0F0F0F);
    return (num << 24) | ((num & 0xFF00) << 8) | ((num >> 8) & 0xFF00) | (num >> 24);
}