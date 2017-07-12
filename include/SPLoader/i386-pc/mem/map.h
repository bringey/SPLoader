/*
** File: include/SPLoader/i386-pc/mem/map.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_MEM_MAP_H
#define _SPLOADER_I386_PC_MEM_MAP_H

#include <SPLoader/i386-pc/mem/MemMap.h>


#define MEMMAP_LOCATION        0x4000
#define MEMMAP_LENGTH          (MEMMAP_LOCATION + 0x0)
#define MEMMAP_BYTES_PER_ENTRY (MEMMAP_LOCATION + 0x4)
#define MEMMAP_TABLE           (MEMMAP_LOCATION + 0x8)

#define MEMMAP_ENTRY_SIZE 24

#define MEMMAP_SIG 0x534D4150

#ifndef __ASM__

#include <stdbool.h>

void memmap_dump(bool showAvailable);

void memmap_dumpEntry(MemMapEntry *entry);


#endif  // __ASM__

#endif