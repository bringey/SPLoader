/*
** File: include/SPLoader/idt/ISRHandler.h
**
** Author: bringey
**
*/

#ifndef _I386_PC_IDT_ISRHANDLER_H
#define _I386_PC_IDT_ISRHANDLER_H

#ifndef __ASM__

typedef void (*ISRHandler)(int vector, int code);

#endif  // __ASM__

#endif