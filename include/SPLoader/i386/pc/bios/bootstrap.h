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

#ifndef	_SPLOADER_I386_PC_BIOS_BOOTSTRAP_H
#define	_SPLOADER_I386_PC_BIOS_BOOTSTRAP_H

#define BOOT_SEGMENT 0x07C0        // default BIOS addr to load boot sector
#define BOOT_ADDRESS 0x7C00

#define LOADER_LBA_LOCATION 0x1B8 // location of the loader LBA
#define LOADER_SIZE_LOCATION 0x1BC // location of the loader size

#endif
