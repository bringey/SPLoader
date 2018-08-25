/*
** File: include/i386-pc/mem/E820Map.h
**
** Author: bringey
**
*/

#ifndef _I386_PC_MEM_E820MAP_H
#define _I386_PC_MEM_E820MAP_H

#include <i386-pc/mem/E820Entry.h>

#ifndef __ASM__

#include <stdint.h>

struct E820Map_s {

    uint32_t length;           // length of the table
    uint32_t length_per_entry; // actual length per entry per BIOS
    E820Entry *table;          // array of E820Entries

} __attribute__((packed));

typedef struct E820Map_s E820Map;

#endif  // __ASM__

#endif
