/*
** File: include/SPLoader/i386-pc/mem/MemMapEntry.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_MEM_MEMMAPENTRY_H
#define _SPLOADER_I386_PC_MEM_MEMMAPENTRY_H

#ifndef __ASM__

#include <stdint.h>

struct MemMapEntry_s {

    union {
        struct {
            uint64_t base;
            uint64_t length;
        } __attribute__((packed));

        struct {
            uint32_t base_lo;
            uint32_t base_hi;
            uint32_t length_lo;
            uint32_t length_hi;
        } __attribute__((packed));
    };

    uint32_t type;
    uint32_t extended;

} __attribute__((packed));


typedef struct MemMapEntry_s MemMapEntry;

#endif

#endif
