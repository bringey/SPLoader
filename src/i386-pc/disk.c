/*
** File: src/i386-pc/disk.c
**
** Author: bringey
*/

#include <disk.h>

#include <i386/regs.h>
#include <err.h>

#include <i386-pc/realmode.h>


// Stage 0 got the drive parameters and left them at 07C0:8000
#define BIOS_DP ((BiosDp*)0xFC00)

#define BIOS_DAP_SEGMENT 0x7C0
#define BIOS_DAP_OFFSET 0x8020
#define BIOS_DAP ((BiosDap*)0xFC20)
#define BIOS_DAP_SIZE 16

#define BUFFER_SEGMENT 0x2000
#define BUFFER_OFFSET 0x0
#define BUFFER 0x20000

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



int _disk_init(void) {
    // using BIOS, no disk init needed
    return E_SUCCESS;
}


int _disk_read(uint32_t start, uint32_t blocks) {

    BiosDap *dap = BIOS_DAP;
    dap->size = BIOS_DAP_SIZE;

    // do max 127 sectors per transfer
    if (blocks > 127) {
        // to-do
    }

    dap->sectors = blocks;
    dap->bufferOffset = BUFFER_OFFSET;
    dap->bufferSegment = BUFFER_SEGMENT;
    dap->lbaLo = start;
    dap->lbaHi = 0;
    
    Regs32 regs = {0};
    regs.edx = 0x80;
    regs.eax = 0x4200;
    regs.esi = (uint32_t)dap;
    __int32(0x13, &regs);

    if ((regs.eflags & EFLAGS_CF) == EFLAGS_CF) {
        return -1;
    } else {
        return E_SUCCESS;
    }
}


uint32_t _disk_blockSize(void) {
    return BIOS_DP->bytesPerSector;
}

uint8_t* _disk_buffer(void) {
    return (uint8_t*)BUFFER;
}