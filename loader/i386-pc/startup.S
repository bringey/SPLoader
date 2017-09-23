/*
** File: loader/i386-pc/startup.S
**
** Author: Brennan Ringey
**
** Main entry point for loader.bin
**
*/

#include <SPLoader/i386-pc/loader.h>

    .arch i686
    .text
    .globl _start, main, con_putchar_at
    .globl __isr_init
    .globl __nmi_enable

_start:
    cli

    // setup stack and data segments
    xorl    %eax, %eax
    movw    $LOADER_GDT_DATA32, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movw    %ax, %ss

    // reset stack, just in case it got trashed somehow
    movl    $LOADER_STACK_ADDRESS, %ebp
    movl    %ebp, %esp

    call    __nmi_enable                    // re-enable NMIs

    call    main


    // returned from main! do nothing forever
    cli

    #if defined(FE_MENU) || defined(FE_CONSOLE)
    // display a ! in the lower right corner of the console
    pushl   $'!'
    pushl   $24
    pushl   $79
    call    con_putchar_at
    addl    $0xC, %esp
    #endif

_end:
    hlt
    jmp     _end
