/*
** File: include/SPLoader/i386-pc/idt/isr.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_IDT_ISR_H
#define _SPLOADER_I386_PC_IDT_ISR_H

#include <SPLoader/i386-pc/idt/ISRHandler.h>

#ifndef __ASM__

extern ISRHandler IDT_ISR_TABLE[];

int isr_init(void);

int isr_install(int vector, ISRHandler isr, ISRHandler *oldIsr);

void isr_defaultExpectedHandler(int vector, int code);

void isr_defaultUnexpectedHandler(int vector, int code);

void isr_exceptionHandler(int vector, int code);

#endif  // __ASM__

#endif