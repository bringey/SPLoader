/*
** File: src/loader/i386-pc/stage0b/early.S
**
** Author: Brennan Ringey
**
** Description: Entry point for the loader16 image
**
** The bootstrap transfers control here after loading the loader image.
** The early routine is responsible for enabling the A20 gate, setting up the
** the GDT and IDT and entering protected mode.
*/

#include <loader/i386-pc/loader.h>


    .arch i686

    .code16
    .section .loader16, "ax"
    .globl early, begtext, _start

    .globl a20_check, a20_enable, get_mem_map
    .globl die


begtext:
early:
    cli
    movw    $LOADER_SEGMENT, %ax     // update data segment
    movw    %ax, %ds

    movb    $0x80, %al               // disable NMIs
    outb    %al, $0x70

    call    a20_check                // check the status of the A20 gate
    orw     %ax, %ax                 // test result
    jnz     early_a20done            // if 1, A20 is already enabled
    call    a20_enable               // enable A20 gate
early_a20done:

    movw    $0x1003, %ax             // disable blinking foreground colors
    movw    $0x0, %bx                // for the console
    int     $0x10

    /*
    ** Get the system memory map
    */
    call    get_mem_map

    /* 
    ** Move the GDT defined in start_gdt to LOADER_GDT_ADDRESS
    ** Now that A20 is enabled, we can access memory above 1 MiB
    */
    call    __move_gdt

    // Load IDT and GDT

    lidt    early_idt
    lgdt    early_gdt

    // enter protected mode

    movl    %cr0, %eax               // get current CR0
    orl     $1, %eax                 // enable the PE bit
    movl    %eax, %cr0               // update CR0

    .byte 0x66 // 32-bit mode prefix
    .code32
    ljmp    $LOADER_GDT_CODE32, $_start
    .code16



__move_gdt:
    pushw   %bp
    movw    %sp, %bp

    pushw   %es
    pushw   %si
    pushw   %di

    movw    $LOADER_GDT_SEGMENT, %ax
    movw    %ax, %es
    movw    $start_gdt, %si           // source = early_gdt
    movw    $0, %di                   // destination = $LOADER_GDT_SEGMENT:0
    movw    $gdt_len, %cx             // cx = length of early_gdt in bytes
    cld
    rep movsb

    xorw    %ax, %ax
    popw    %di
    popw    %si
    popw    %es
    popw    %bp
    ret



early_idt:
    .word   0x800
    .quad   LOADER_IDT_ADDRESS

early_gdt:
    .word   gdt_len - 1
    .quad   LOADER_GDT_ADDRESS


/*
** The GDT.  This cannot be created in C because the bootstrap is not
** linked with that code.
*/
start_gdt:       // first GDT entry is always null
    .word 0, 0, 0, 0

code32_seg:	     // limit FFFFF, base 0, R/E code seg, 32-bit 4K
    .word 0xFFFF
    .word 0x0000
    .byte   0x00
    .byte   0x9A // 1 00 1 1010: present, prio 0, C/D, R/E code
    .byte   0xCF // 1 1 00 1111: 4K, 32-bit, 0, 0, limit[19:16]
    .byte   0x00

data32_seg:	     // limit FFFFF, base 0, R/W data seg, 32-bit 4K
    .word 0xFFFF
    .word 0x0000
    .byte   0x00
    .byte   0x92 // 1 00 1 0010: present, prio 0, C/D, R/W data
    .byte   0xCF
    .byte   0x00

//
// 16-bit code and data segments
// These segments are used if a return to real mode is needed
//

code16_seg:
    .word 0xFFFF
    .word 0x0000
    .byte   0x00 
    .byte   0x9A
    .byte   0x0F
    .byte   0x00

data16_seg:
    .word 0xFFFF
    .word 0x0000
    .byte   0x00
    .byte   0x92
    .byte   0x0F
    .byte   0x00

end_gdt:
gdt_len = end_gdt - start_gdt
