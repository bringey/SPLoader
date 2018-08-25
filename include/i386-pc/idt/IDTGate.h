/*
** File: include/SPLoader/i386-pc/idt/IDTGate.h
**
** Author: bringey
**
*/

#ifndef _I386_PC_IDT_IDTGATE_H
#define _I386_PC_IDT_IDTGATE_H

/*
** IDT Descriptors
** 
** IA-32 V3, page 5-13.
**
** All have a segment selector in bytes 2 and 3; Task Gate descriptors
** have bytes 0, 1, 4, 6, and 7 reserved; others have bytes 0, 1, 6,
** and 7 devoted to the 16 bits of the Offset, with the low nybble of
** byte 4 reserved.
*/
#define IDT_PRESENT     0x8000
#define IDT_DPL_MASK    0x6000
#define IDT_DPL_0       0x0000
#define IDT_DPL_1       0x2000
#define IDT_DPL_2       0x4000
#define IDT_DPL_3       0x6000
#define IDT_GATE_TYPE   0x0F00
#define IDT_TASK_GATE   0x0500
#define IDT_INT16_GATE  0x0600
#define IDT_INT32_GATE  0x0E00
#define IDT_TRAP16_GATE 0x0700
#define IDT_TRAP32_GATE 0x0F00

#ifndef __ASM__

#include <stdint.h>

/*
** Structure definition for an IDT task, interrupt or trap gate.
**
** 31                   16 15                    0
** +----------------------+----------------------+
** | offset_hi            | flags                | 4
** +----------------------+----------------------+
** | selector             | offset_lo            | 0
** +----------------------+----------------------+
**
** Task Gate
**   .selector = TSS Segment selector
**   .flags = IDT_PRESENT | DPL | IDT_TASK_GATE
**
** Interrupt Gate
**    .offset_lo = offset & 0xFFFF
**    .selector = segment selector
**    .flags = IDT_PRESENT | DPL | IDT_INT<16|32>_GATE
**    .offset_hi = offset >> 16
**
** Trap Gate
**     .offset_lo = offset & 0xFFFF
**     .selector = segment selector
**     .flags = IDT_PRESENT | DPL | IDT_TRAP<16|32>_GATE
**     .offset_hi = offset >> 16
*/
typedef struct IDTGate_s {

    uint16_t offset_lo;
    uint16_t selector;
    uint16_t flags;
    uint16_t offset_hi;

} IDTGate;

#endif  // __ASM__

#endif