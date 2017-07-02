/*
** File: loader/i386-pc/console/_driver.S
**
** Author: Brennan Ringey
**
** Contains function implementations defined in SPLoader/console/driver.h
** for the i386-pc platform.
**
*/



#define __ASM__
#include <SPLoader/err.h>
#include "constants.h"

    .arch i686
    .text
    .globl con_driver_clear


con_driver_clear:
    pushl   %ebp
    movl    %esp, %ebp
    pushl   %ecx
    pushl   %edi

    movl    $(VGA_WIDTH * VGA_HEIGHT), %ecx  // ecx = the entire text buffer
    movl    $(VGA_NULLCELL), %eax
    movl    $(VGA_BUFFER), %edi              // edi = text buffer
    cld
    rep stosw

    popl    %edi
    popl    %ecx
    popl    %ebp
    movl    $E_SUCCESS, %eax
    ret