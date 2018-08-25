/*
** File: include/SPLoader/i386-pc/mem/E820Entry.h
**
** Author: bringey
**
*/

#ifndef _I386_PC_MEM_E820ENTRY_H
#define _I386_PC_MEM_E820ENTRY_H

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

#endif

#endif
