/*
** File: include/i386/regs.h
**
** Author: bringey
**
*/


#ifndef _I386_REGS_H
#define _I386_REGS_H


/*
** Bit definitions in registers
**
** See IA-32 Intel Architecture SW Dev. Manual, Volume 3: System
** Programming Guide, page 2-8.
*/

/*
** EFLAGS
*/
#define EFLAGS_RSVD     0xFFC00000	/* reserved */
#define EFLAGS_MB0      0x00008020	/* must be zero */
#define EFLAGS_MB1      0x00000002	/* must be 1 */

#define EFLAGS_ID       0x00200000
#define EFLAGS_VIP      0x00100000
#define EFLAGS_VIF      0x00080000
#define EFLAGS_AC       0x00040000
#define EFLAGS_VM       0x00020000
#define EFLAGS_RF       0x00010000
#define EFLAGS_NT       0x00004000
#define EFLAGS_IOPL     0x00003000
#define EFLAGS_OF       0x00000800
#define EFLAGS_DF       0x00000400
#define EFLAGS_IF       0x00000200
#define EFLAGS_TF       0x00000100
#define EFLAGS_SF       0x00000080
#define EFLAGS_ZF       0x00000040
#define EFLAGS_AF       0x00000010
#define EFLAGS_PF       0x00000004
#define EFLAGS_CF       0x00000001

/*
** CR0, CR1, CR2, CR3, CR4
**
** IA-32 V3, page 2-12.
*/
#define CR0_RSVD        0x1ffaffc0
#define CR0_PG          0x80000000
#define CR0_CD          0x40000000
#define CR0_NW          0x20000000
#define CR0_AM          0x00040000
#define CR0_WP          0x00010000
#define CR0_NE          0x00000020
#define CR0_ET          0x00000010
#define CR0_TS          0x00000008
#define CR0_EM          0x00000004
#define CR0_MP          0x00000002
#define CR0_PE          0x00000001
#define CR1_RSVD        0xffffffff
#define CR2_RSVD        0x00000000
#define CR2_PF_LIN_ADDR 0xffffffff
#define CR3_RSVD        0x00000fe7
#define CR3_PD_BASE     0xfffff000
#define CR3_PCD         0x00000010
#define CR3_PWT         0x00000008
#define CR4_RSVD        0xfffff800
#define CR4_OSXMMEXCPT  0x00000400
#define CR4_OSFXSR      0x00000200
#define CR4_PCE         0x00000100
#define CR4_PGE         0x00000080
#define CR4_MCE         0x00000040
#define CR4_PAE         0x00000020
#define CR4_PSE         0x00000010
#define CR4_DE          0x00000008
#define CR4_TSD         0x00000004
#define CR4_PVI         0x00000002
#define CR4_VME         0x00000001


#ifndef __ASM__

#include <inline.h>

// the following functions are inline since they only require 1 or 2
// instructions.

static INLINE int __eflags(void) {
    int flags;
    asm volatile (
        "pushfl;"
        "popl    %0;"
        : "=r"(flags)
        :
        :
    );
    return flags;
}

static INLINE int __esp(void) {
    int esp;
    asm volatile (
        "movl   %%esp, %0;"
        : "=r"(esp)
        :
        :
    );
    return esp;
}

static INLINE int __ebp(void) {
    int ebp;
    asm volatile (
        "movl   %%ebp, %0"
        : "=r"(ebp)
        :
        :
    );
    return ebp;
}


#endif  // __ASM__

#endif