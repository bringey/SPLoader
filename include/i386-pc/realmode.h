
#ifndef _SPLOADER_I386_PC_REALMODE_H
#define _SPLOADER_I386_PC_REALMODE_H

#define INT32_LOCATION 0x5000

#ifndef __ASM__

#include <i386-pc/Regs32.h>


void __int32_init(void);

//
// Call a BIOS function from protected mode.
//
void __int32(int vector, Regs32 *regs);


#endif

#endif