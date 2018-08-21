/*
** File: include/SPLoader/idt/ISRHandler.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_I386_PC_IDT_ISRHANDLER_H
#define _SPLOADER_I386_PC_IDT_ISRHANDLER_H

#ifndef __ASM__

typedef void (*ISRHandler)(int vector, int code);

#endif  // __ASM__

#endif