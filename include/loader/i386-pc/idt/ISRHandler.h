/*
** File: include/loader/i386-pc/idt/ISRHandler.h
**
** Author: bringey
**
*/

#ifndef _LOADER_I386_PC_IDT_ISRHANDLER_H
#define _LOADER_I386_PC_IDT_ISRHANDLER_H

#ifndef __ASM__

typedef void (*ISRHandler)(int vector, int code);

#endif  // __ASM__

#endif