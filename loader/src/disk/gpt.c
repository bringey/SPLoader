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
#include <loader/mem.h>
#include <loader/string.h>

#define TABLE_MAX 128

static int check(Disk *disk, GptHeader *header, uint64_t lba, void **buf);
static bool iszero(void *buf, size_t count);


// TODO: actually test this, not able to test this at time of writing due to
//       tooling not implemented
int disk_gpt_read(DiskLabel *label) {

    // allocate the partition table
    // we use 128 as a fixed limit similar to other OSes, it should be noted
    // that GPT can support more than this.
    label->table = (DiskPart*)mem_malloc(sizeof(DiskPart) * TABLE_MAX);

    Disk *disk = label->disk;
    GptHeader header;
    void *buf = NULL;

    int err = check(disk, &header, GPT_LBA_HEADER, &buf);
    if (err == E_GPT_HEADER_INTEGRITY || err == E_GPT_PARTITION_INTEGRITY) {
        // try the backup header
        err = check(disk, &header, header.alternateLba, &buf);
    }
    
    if (err != E_SUCCESS) {
        // invalid GPT
        return err;
    }

    // convert the gpt partition table into label->table
    size_t nparts = 0;
    DiskPart *table = label->table;
    uint32_t entryCount = header.partitionEntryCount;
    uint32_t entrySize = header.partitionEntrySize;
    GptPartitionEntry *entry = (GptPartitionEntry*)buf;
    for (uint32_t i = 0; i != entryCount; ++i) {
        if (!iszero(entry, entrySize)) {
            // partition entry is not zero'd out, attempt to read it
            table->num = i;
            table->active = (entry->attributes & GPT_ATTRIBUTE_LEGACY) == GPT_ATTRIBUTE_LEGACY;
            table->startLba = entry->startingLba;
            table->endLba = entry->endingLba;
            ++nparts;
            ++table;
            if (nparts == TABLE_MAX) {
                // we have reached the fixed limit, stop here.
                break;
            }
        }
        entry = (GptPartitionEntry*)((uint8_t*)entry + entrySize);
    }


    mem_free(buf);

    return E_FAILURE;
}

int check(Disk *disk, GptHeader *header, uint64_t lba, void **buf) {

    // check the signature
    if (header->signature != GPT_SIGNATURE) {
        return E_GPT_HEADER_INVALID;
    }

    // check the version
    if (header->revision != GPT_VERSION_1) {
        return E_GPT_VERSION;
    }

    // 2. check the header CRC
    uint32_t headerCrc = header->headerCrc32;
    // zero the header crc field (must be zero when calculating the CRC)
    header->headerCrc32 = 0;
    uint32_t testCrc = spl_crc32(header, header->headerSize);
    // restore the old crc value back in header
    header->headerCrc32 = headerCrc;
    if (headerCrc != testCrc) {
        return E_GPT_HEADER_INTEGRITY;
    }

    // 3. check that header->myLba == lba
    if (header->myLba != lba) {
        return E_GPT_HEADER_INTEGRITY;
    }

    // 4. check the partition table crc
    uint32_t entryCount = header->partitionEntryCount;
    uint32_t entrySize = header->partitionEntrySize;
    size_t bufsize = entryCount * entrySize;
    if (*buf == NULL) {
        // table buffer is null, alloc it
        *buf = mem_malloc(bufsize);
    }
    disk_read(disk, *buf, lba, entrySize, entryCount);
    uint32_t tableCrc = spl_crc32(*buf, bufsize);
    if (tableCrc != header->partitionEntryCrc32) {
        return E_GPT_PARTITION_INTEGRITY;
    }


    return E_SUCCESS;
}

bool iszero(void *buf, size_t count) {
    uint8_t *bbuf = (uint8_t*)buf;
    int sum = 0;
    for (size_t i = 0; i != count; ++i) {
        sum |= bbuf[i];
    }
    return sum == 0;
}

#else

typedef int dummy;

#endif