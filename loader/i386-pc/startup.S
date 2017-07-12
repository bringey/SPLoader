/*
** File: loader/i386-pc/startup.S
**
** Author: Brennan Ringey
**
** Main entry point for loader.bin
**
*/

#include <SPLoader/loader.h>

    .arch i686
    .text
    .globl _start, main, con_init, con_putchar_at
    .globl isr_init

_start:
    cli
    movb    $0x0, %al               // re-enable NMIs
    outb    $0x70

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

    // initialize console
    call    con_init

    // initialize interrupts
    call    isr_init
    sti

    call    main


    // returned from main! do nothing forever
    cli

    // display a ! in the lower right corner of the console
    pushl   $'!'
    pushl   $24
    pushl   $79
    call    con_putchar_at
    addl    $0xC, %esp

_end:
    hlt
    jmp     _end
