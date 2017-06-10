/*
** SCCS ID:	@(#)bootstrap.h	1.4	10/18/09
**
** File:	bootstrap.h
**
** Author:	K. Reek
**
** Contributor:	Warren R. Carithers, Garrett C. Smith
**
** Description:	Addresses where various stuff goes in memory.
*/

#ifndef	_BOOTSTRAP_H
#define	_BOOTSTRAP_H

/*
** The target program itself
*/
#define TARGET_SEGMENT	0x00001000	/* 0001:0000 */
#define TARGET_ADDRESS	0x00010000	/* and upward */
#define	TARGET_STACK	0x00010000	/* and downward */

/*
** The Global Descriptor Table (0000:0500 - 0000:2500)
*/
#define	GDT_SEGMENT	0x00000050
#define	GDT_ADDRESS	0x00000500

	/* segment register values */
#define	GDT_LINEAR	0x0008		/* All of memory, R/W */
#define	GDT_CODE	0x0010		/* All of memory, R/E */
#define	GDT_DATA	0x0018		/* All of memory, R/W */
#define	GDT_STACK	0x0020		/* All of memory, R/W */
#define GDT_CODE16	0x0028
#define GDT_DATA16	0x0030

// GDT used by the early initialization routine
#define EARLY_GDT_SEGMENT 0x02D1
#define EARLY_GDT_ADDRESS 0x2D10

#define EARLY_GDT_CODE32 0x08
#define EARLY_GDT_DATA32 0x10
#define EARLY_GDT_CODE16 0x18
#define EARLY_GDT_DATA16 0x20


/*
** The Interrupt Descriptor Table (0000:2500 - 0000:2D00)
*/
#define	IDT_SEGMENT	0x00000250
#define IDT_ADDRESS	0x00002500

/*
** Physical Memory Map Table (0000:2D00 - 0000:2D08)
*/
#define	MMAP_SEGMENT	0x000002D0
#define	MMAP_ADDRESS	0x00002D00
#define	MMAP_EXT_LO	0x00	/* extended memory - low register */
#define	MMAP_EXT_HI	0x02	/* extended memory - high register */
#define	MMAP_CFG_LO	0x04	/* configured memory - low register */
#define	MMAP_CFG_HI	0x06	/* configured memory - high register */
#define	MMAP_PROGRAMS	0x08	/* # of programs loaded from disk (+ kernel) */
#define	MMAP_SECTORS	0x0a	/* table of sector counts for each program */


/*
** realprog address (0000:2E00 - 0000:2FFF)
*/

#define REALPROG_SEGMENT 0x0300
#define REALPROG_ADDRESS 0x3000

/*
** Real Mode Program(s) Text Area (0000:3000 - 0x7c00)
*/

#define	RMTEXT_SEGMENT	0x00000300
#define	RMTEXT_ADDRESS	0x00003000

#endif
