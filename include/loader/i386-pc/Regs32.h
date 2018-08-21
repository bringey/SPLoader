
#ifndef _SPLOADER_I386_REGS32_H
#define _SPLOADER_I386_REGS32_H

// size in bytes of Regs32 struct
#define REGS32_SIZE 44

#ifndef __ASM__

#include <stdint.h>

struct Regs32_s {

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint16_t gs;
    uint16_t fs;
    uint16_t es;
    uint16_t ds;
    uint32_t eflags;

} __attribute__((packed));

typedef struct Regs32_s Regs32;

#endif

#endif