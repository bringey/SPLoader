
#ifndef _SPLOADER_I386_PC_MEM_MEMMAP_H
#define _SPLOADER_I386_PC_MEM_MEMMAP_H

#ifndef __ASM__

#include <stdint.h>

typedef struct MemMap_s {

    uint32_t length;           // length of the table
    uint32_t length_per_entry; // actual length per entry per BIOS
    MemMapEntry *table;        // array of MemMapEntries

} MemMap;

#endif

#endif
