/*
** File: src/i386-pc/disk.c
**
** Author: bringey
*/

#include <loader/assert.h>
#include <loader/disk.h>

#include <loader/i386/regs.h>
#include <loader/err.h>

#include <loader/i386-pc/loader.h>
#include <loader/i386-pc/realmode.h>


// Stage 0 got the drive parameters and left them at 07C0:8000
#define BIOS_DP ((BiosDp*)LOADER_DP_ADDRESS)

#define BIOS_DAP ((BiosDap*)LOADER_DAP_ADDRESS)
#define BIOS_DAP_SIZE 16


#define MAX_BLOCKS_PER_TRANSFER 127

struct BiosDap_s {
    uint16_t size;
    uint16_t sectors;
    uint16_t bufferOffset;
    uint16_t bufferSegment;
    uint32_t lbaLo;
    uint32_t lbaHi;
} __attribute__((packed));

struct BiosDp_s {
    uint16_t size;
    uint16_t flags;
    uint32_t cylinders;
    uint32_t heads;
    uint32_t sectorsPerTrack;
    uint64_t sectors;
    uint16_t bytesPerSector;
    uint32_t eddPtr;
} __attribute__((packed));

typedef struct BiosDp_s BiosDp;

typedef struct BiosDap_s BiosDap;

int _disk_info(DiskInfo *info) {
    assert(info != NULL);
    info->totalBlocks = BIOS_DP->sectors;
    info->blocksize = BIOS_DP->bytesPerSector;
    // Some BIOSes limit the max sectors to read to 127
    // 127 * 512 (typical sector size) = 0xFE00 (about one segment)
    // since we use the the segment 2000:0000 for the buffer, we will follow
    // this limit
    info->maxBlocksPerRead = MAX_BLOCKS_PER_TRANSFER;
    info->buffer = (uint8_t*)LOADER_DD_BUFFER;
    // don't need any auxillary data for this driver
    info->aux = NULL;
    return E_SUCCESS;
}

int _disk_init(void) {
    // nothing to init, using BIOS
    return E_SUCCESS;
}


int _disk_read(Disk *disk, uint32_t start, uint32_t blocks) {
    (void)disk;

    BiosDap *dap = BIOS_DAP;
    dap->size = BIOS_DAP_SIZE;

    dap->sectors = blocks;
    dap->bufferOffset = LOADER_DD_OFFSET;
    dap->bufferSegment = LOADER_DD_SEGMENT;
    dap->lbaLo = start;
    dap->lbaHi = 0;
    
    Regs32 regs = {0};
    regs.edx = 0x80;
    regs.eax = 0x4200;
    regs.esi = (uint32_t)dap;
    __int32(0x13, &regs);

    if ((regs.eflags & EFLAGS_CF) == EFLAGS_CF) {
        return E_DISK_READ;
    } else {
        return E_SUCCESS;
    }
}
