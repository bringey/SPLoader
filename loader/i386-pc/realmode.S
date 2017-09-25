
#define __ASM__
#include <SPLoader/i386-pc/loader.h>
#include <SPLoader/i386-pc/realmode.h>
#include <SPLoader/i386-pc/Regs32.h>

#define RELOC(label) ((label - __int32_reloc) + INT32_LOCATION)


    .arch i686
    .text

    .globl __int32_init, __int32

__int32_init:
    pushl   %esi
    pushl   %edi

    movl    $__int32_reloc, %esi
    movl    $INT32_LOCATION, %edi
    movl    $(__int32_reloc_end - __int32_reloc), %ecx
    cld
    rep movsb

    popl    %edi
    popl    %esi
    ret


__int32:
    // context-save, save all registers and the IDTR
    pushal
    pushw   %ds
    pushw   %es
    pushw   %fs
    pushw   %gs
    sidt    -0x6(%esp)

    // Stack Structure:
    //       +--------------+
    //       | Regs32 *regs |
    //  0x30 +--------------+
    //       | int vector   |
    //  0x2C +--------------+
    //       | return addr  |
    //  0x28 +--------------+
    //       |              |
    //       | GP Regs      |
    //       |              |
    //   0x8 +--------------+
    //       | Seg Regs     |
    //       |              |
    //   0x0 +--------------+ <----- %esp
    //       | IDTR         |
    //  -0x6 +--------------+
    // 
    // ======================= 32-bit /\ ======================================
    // ======================= 16-bit \/ ======================================
    //
    //       +--------------+
    //       |              |
    //       | Regs32 regs  |
    //       |              |
    //       |              |
    // -0x32 +--------------+ <----- %sp

    movl    0x2C(%esp), %eax                // get our vector
    movb    %al, RELOC(__ib)                // store it in the int operand

    movl    0x30(%esp), %esi                // get the regs pointer
    leal    -0x32(%esp), %edi               // dest is right below the IDTR
    movl    %edi, %esp                      // esp now points to the 16-bit stack
    movl    $REGS32_SIZE, %ecx
    cld
    rep movsb                               // do the copy

    // jump to the relocated portion
    ljmpw   $LOADER_GDT_CODE16, $INT32_LOCATION

__int32_exit:
    xorl    %eax, %eax
    movw    $LOADER_GDT_DATA32, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movw    %ax, %ss
    
    movl    %esp, %esi                      // source: 16-bit stack
    leal    0x32(%esp), %esp                // revert to the 32-bit stack
    movl    0x30(%esp), %esi                // dest: regs pointer
    movl    $REGS32_SIZE, %ecx
    cld
    rep movsb

    lidt    -0x6(%esp)                      // restore the IDTR
    popw    %gs
    popw    %fs
    popw    %es
    popw    %ds
    popal
    ret


__int32_reloc:
    .code16
    movw    $LOADER_GDT_DATA16, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movw    %ax, %ss

    lidt    RELOC(__realidt)

    movl    %cr0, %eax                      // turn off PE bit in %cr0
    andb    $(~1), %al
    movl    %eax, %cr0
    jmp     $0, $RELOC(__rmode16)

__rmode16:
    xorw    %ax, %ax
    movw    %ax, %ss
    popal
    popw    %gs
    popw    %fs
    popw    %es
    popw    %ds
    addw    $4, %sp                         // "pop" the flags
    sti
      .byte 0xCD
__ib: .byte 0x0
    cli
    pushfl
    pushw   %ds
    pushw   %es
    pushw   %fs
    pushw   %gs
    pushal

    movl    %cr0, %eax
    orl     $1, %eax
    movl    %eax, %cr0
    ljmpl   $LOADER_GDT_CODE32, $__int32_exit


__realidt:
    .word   0x3FF
    .long   0x0

__int32_reloc_end:
