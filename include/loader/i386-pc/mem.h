/*
** File: include/loader/i386-pc/mem.h
**
** Author: bringey
**
** Macros and typedefs for the i386-pc memory driver
*/


#ifndef _LOADER_I386_PC_MEM_H
#define _LOADER_I386_PC_MEM_H

#define MMAP_ENTRY_SIZE 24
#define MMAP_SIG 0x534D4150


#ifndef __ASM__

#include <stdint.h>

struct E820Entry_s {

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

typedef struct E820Entry_s E820Entry;

struct E820Map_s {

    uint32_t length;           // length of the table
    uint32_t length_per_entry; // actual length per entry per BIOS
    E820Entry *table;          // array of E820Entries

} __attribute__((packed));

typedef struct E820Map_s E820Map;

#endif


#endif