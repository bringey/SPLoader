/*
** File: loader/i386/pc/startup.S
**
** Author: Brennan Ringey
**
** Main entry point for loader.bin
**
*/

#include <SPLoader/loader.h>

	.arch i686
	.text
	.globl _start, main

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

	movl    $LOADER_STACK, %ebp     // setup stack
	movl    %ebp, %esp

	// initialize console

	// initialize interrupts


	call    main
