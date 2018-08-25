/*
** File: src/i386-pc/startup.S
**
** Author: bringey
**
** Main entry point for loader.bin
**
*/

#include <i386-pc/loader.h>

    .arch i686
    .text
    .globl _start, main
    .globl __int32_init
    .globl spl_con_init

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

    call    spl_con_init

    call    __int32_init

    call    main


    // returned from main! do nothing forever
    cli

    #if 0
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
