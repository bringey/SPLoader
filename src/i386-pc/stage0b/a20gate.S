/*
** File: src/i386-pc/stage0b/a20gate.S
**
** Author: Brennan Ringey
**
** Description: Contains routines for enabline the A20 gate
**
*/

    .arch i686
    .code16

    .section .loader16, "ax"

    .globl a20_check, a20_enable

a20_check:
    pushw   %bp
    movw    %sp, %bp

    pushf
    pushw   %ds
    pushw   %es
    pushw   %fs
    pushw   %gs

    xorw    %ax, %ax
    movw    %ax, %es

    notw    %ax
    movw    %ax, %ds

    movw    $0x500, %di
    movw    $0x510, %si

    movb    %es:(%di), %al
    pushw   %ax

    movb    %ds:(%si), %al
    pushw   %ax

    movb    $0, %es:(%di)
    movb    $0xFF, %ds:(%si)

    cmpb    $0xFF, %es:(%di)

    popw    %ax
    movb    %al, %ds:(%si)

    popw    %ax
    movb    %al, %es:(%di)

    movw    $0, %ax
    je      a20_check_exit
    movw    $1, %ax

a20_check_exit:
    popw    %gs
    popw    %fs
    popw    %es
    popw    %ds
    popf

    popw    %bp
    ret

/*
** Enable the A20 gate for full memory access.
*/
a20_enable:
    call    a20wait
    movb    $0xad, %al
    outb    %al, $0x64

    call    a20wait
    movb    $0xd0, %al
    outb    %al, $0x64

    call    a20wait2
    inb     $0x60, %al
    pushl   %eax

    call    a20wait
    movb    $0xd1, %al
    outb    %al, $0x64

    call    a20wait
    popl    %eax
    orb     $2, %al
    outb    %al, $0x60

    call    a20wait
    mov     $0xae, %al
    out     %al, $0x64

    call    a20wait
    ret

a20wait: /* wait until bit 1 of the device register is clear */
    movl    $65536, %ecx         // loop a lot if need be
a20wait_loop:
    inb     $0x64, %al           // grab the byte
    test    $2, %al              // is the bit clear?
    jz      a20wait_exit            // yes
    loop    a20wait_loop            // no, so loop
    jmp     a20wait              // if still not clear, go again
a20wait_exit:
    ret

a20wait2: /* like a20wait, but waits until bit 0 is set. */
    mov     $65536, %ecx
a20wait2_loop:
    in      $0x64, %al
    test    $1, %al
    jnz     a20wait2_exit
    loop    a20wait2_loop
    jmp     a20wait2
a20wait2_exit:
    ret