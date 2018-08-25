/*
** File: src/core/i386/io.S
**
** Author: Brennan Ringey
**
*/

    .arch i686
    .text

    .globl __inb, __inw, __inl, __outb, __outw, __outl


__inb:
    pushl   %ebp
    movl    %esp, %ebp
    
    xorl    %eax, %eax
    movl    8(%ebp), %edx
    inb     (%dx)

    popl    %ebp
    ret

__inw:
    pushl   %ebp
    movl    %esp, %ebp
    
    xorl    %eax, %eax
    movl    8(%ebp), %edx
    inw     (%dx)

    popl    %ebp
    ret

__inl:
    pushl   %ebp
    movl    %esp, %ebp
    
    xorl    %eax, %eax
    movl    8(%ebp), %edx
    inl     (%dx)

    popl    %ebp
    ret


__outb:
    pushl   %ebp
    movl    %esp, %ebp

    movl    8(%ebp), %edx
    movl    12(%ebp), %eax
    outb    (%dx)

    popl    %ebp
    ret

__outw:
    pushl   %ebp
    movl    %esp, %ebp

    movl    8(%ebp), %edx
    movl    12(%ebp), %eax
    outw    (%dx)

    popl    %ebp
    ret

__outl:
    pushl   %ebp
    movl    %esp, %ebp

    movl    8(%ebp), %edx
    movl    12(%ebp), %eax
    outl    (%dx)

    popl    %ebp
    ret
