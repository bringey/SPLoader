/*
** File: include/loader/i386-pc/realmode.h
**
** Author: bringey
*/

#ifndef _LOADER_I386_PC_REALMODE_H
#define _LOADER_I386_PC_REALMODE_H

#ifndef __ASM__

#include <loader/i386-pc/Regs32.h>


void __int32_init(void);

//
// Call a BIOS function from protected mode.
//
void __int32(int vector, Regs32 *regs);


#endif

#endif