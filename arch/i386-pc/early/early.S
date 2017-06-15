/*
** File: loader/early/early.S
**
** Author: Brennan Ringey
**
** Description: Entry point for the loader image
**
** The bootstrap transfers control here after loading the loader image.
** The early routine is responsible for enabling the A20 gate, setting up the
** GDT and IDT and entering protected mode.
*/

	.arch i686

	.code16
	.text
	.globl early, begtext

	.globl a20_check, a20_enable


begtext:
early:
	cli
	call    a20_check
	hlt
	orw     %ax, %ax                 // test result
	jnz     early_a20done            // if 1, A20 is already enabled
	call    a20_enable               // enable A20 gate

early_a20done:
	hlt

