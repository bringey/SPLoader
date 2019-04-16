/*
** File: loader/i386-pc/early/memmap.S
**
** Contains routines for getting the system memory map
**
*/

#define __ASM__
#include <loader/i386-pc/loader.h>
#include <loader/i386-pc/mem.h>

    .arch i686
    .code16
    .section .loader16, "ax"
    .globl get_mem_map

get_mem_map:
    pushw   %bp
    movw    %sp, %bp

    pushw   %es
    pushl   %ebx
    pushl   %ecx
    pushl   %edx
    pushl   %esi                        // esi is used as a counter
    pushl   %edi

    xorl    %esi, %esi                  // set counter to 0
    xorl    %ebx, %ebx                  // EBX = 0
    movw    %bx, %es                    // es = 0
    movl    $LOADER_MMAP_TABLE, %edi    // di = MEMMAP_LOCATION
    

__loop:
    movl    $0xE820, %eax               // EAX = 0xE820
    movl    $MMAP_ENTRY_SIZE, %ecx
    movl    $MMAP_SIG, %edx             // EDX = signature ('SMAP')
    int     $0x15

    jc      __loopend                   // error occurred if CF is set

    incl    %esi                        // increment counter
    addl    $MMAP_ENTRY_SIZE, %edi      // increment pointer

    testl   %ebx, %ebx                  // test continuation, if 0 exit loop
    jnz     __loop
__loopend:

    movl    %esi, %es:LOADER_MMAP_LENGTH        // save the length of the table
    movl    %ecx, %es:LOADER_MMAP_ENTRY_SIZE

    popl    %edi
    popl    %esi
    popl    %edx
    popl    %ecx
    popl    %ebx
    popw    %es
    popw    %bp
    ret