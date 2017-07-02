/*
** File: include/SPLoader/i386-pc/mem/MemMap.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_MEM_MEMMAP_H
#define _SPLOADER_I386_PC_MEM_MEMMAP_H

#ifndef __ASM__

#include <stdint.h>
#include <SPLoader/i386-pc/mem/MemMapEntry.h>

struct MemMap_s {

    uint32_t length;           // length of the table
    uint32_t length_per_entry; // actual length per entry per BIOS
    MemMapEntry *table;        // array of MemMapEntries

} __attribute__((packed));

typedef struct MemMap_s MemMap;

#endif

#endif
