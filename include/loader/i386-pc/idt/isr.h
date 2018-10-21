/*
** File: include/loader/i386-pc/idt/isr.h
**
** Author: bringey
**
*/

#ifndef _LOADER_I386_PC_IDT_ISR_H
#define _LOADER_I386_PC_IDT_ISR_H

#include <loader/i386-pc/idt/ISRHandler.h>

//
// Exception vectors (0x00-0x1F)
//
#define	INT_VEC_DIVIDE_ERROR            0x00
#define	INT_VEC_DEBUG_EXCEPTION         0x01
#define	INT_VEC_NMI_INTERRUPT           0x02
#define	INT_VEC_BREAKPOINT              0x03
#define	INT_VEC_INTO_DETECTED_OVERFLOW  0x04
#define	INT_VEC_BOUND_RANGE_EXCEEDED    0x05
#define	INT_VEC_INVALID_OPCODE          0x06
#define	INT_VEC_DEVICE_NOT_AVAILABLE    0x07
#define	INT_VEC_DOUBLE_EXCEPTION        0x08
#define	INT_VEC_COPROCESSOR_OVERRUN     0x09
#define	INT_VEC_INVALID_TSS             0x0A
#define	INT_VEC_SEGMENT_NOT_PRESENT     0x0B
#define	INT_VEC_STACK_FAULT             0x0C
#define	INT_VEC_GENERAL_PROTECTION      0x0D
#define	INT_VEC_PAGE_FAULT              0x0E
// vector 0x0F is reserved, do not use
#define	INT_VEC_COPROCESSOR_ERROR       0x10
#define	INT_VEC_ALIGNMENT_CHECK         0x11
#define	INT_VEC_MACHINE_CHECK           0x12
#define	INT_VEC_SIMD_FP_EXCEPTION       0x13
#define INT_VEC_VIRT_EXCEPTION          0x14
// vectors 0x15-0x1F are reserved, do not use

//
// Master PIC vectors (0x20-0x27)
//
#define	INT_VEC_TIMER                   0x20  // IRQ#0
#define	INT_VEC_KEYBOARD                0x21  // IRQ#1
// IRQ#2 cascaded
#define	INT_VEC_SERIAL_PORT_2           0x23  // IRQ#3
#define	INT_VEC_SERIAL_PORT_1           0x24  // IRQ#4
#define	INT_VEC_PARALLEL_PORT_2         0x25  // IRQ#5
#define	INT_VEC_FLOPPY_DISK             0x26  // IRQ#6
#define	INT_VEC_PARALLEL_PORT_1         0x27  // IRQ#7
//
// Slave PIC vectors (0x28-0x2F)
//
#define INT_VEC_RTC                     0x28  // IRQ#8
#define INT_VEC_ACPI                    0x29  // IRQ#9
#define INT_VEC_PERIPHERAL_1            0x2A  // IRQ#10
#define INT_VEC_PERIPHERAL_2            0x2B  // IRQ#11
#define	INT_VEC_MOUSE                   0x2C  // IRQ#12
#define INT_VEC_COPROCESSOR             0x2D  // IRQ#13
#define INT_VEC_PRIMARY_ATA             0x2E  // IRQ#14
#define INT_VEC_SECONDARY_ATA           0x2F  // IRQ#15

//
// User vectors (0x30-0xFF)
//

#ifndef __ASM__

extern ISRHandler IDT_ISR_TABLE[];

int __isr_init(void);

int __isr_install(int vector, ISRHandler isr, ISRHandler *oldIsr);

void __isr_defaultExpectedHandler(int vector, int code);

void __isr_defaultUnexpectedHandler(int vector, int code);

void __isr_exceptionHandler(int vector, int code);

#endif  // __ASM__

#endif