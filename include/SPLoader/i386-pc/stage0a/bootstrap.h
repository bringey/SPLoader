/*
** File: include/SPLoader/i386-pc/stage0a/bootstrap.h
**
** Author: Brennan Ringey
**
** Description:	Error codes and locations for the MBR bootstrap
*/

#ifndef	_SPLOADER_I386_PC_STAGE0A_BOOTSTRAP_H
#define	_SPLOADER_I386_PC_STAGE0A_BOOTSTRAP_H

#define BOOT_SEGMENT 0x07C0        // default BIOS addr to load boot sector
#define BOOT_ADDRESS 0x7C00

#define LOADER_LBA_LOCATION    0x1B4 // location of the loader LBA
#define LOADER_SIZE_LOCATION   0x1B8 // location of the loader size
#define LOADER16_SIZE_LOCATION 0x1BA // location of the loader16 size

// error codes

#define E_BOOTSTRAP_DEFAULT  '0'
#define E_BOOTSTRAP_DISK     '1'
#define E_BOOTSTRAP_NOLBA    '2'
#define E_BOOTSTRAP_TOOBIG   '3'
#define E_BOOTSTRAP_TOOSMALL '4'

// debug codes

#define E_BOOTSTRAP_DEBUG1   'X'
#define E_BOOTSTRAP_DEBUG2   'Y'
#define E_BOOTSTRAP_DEBUG3   'Z'

#ifndef __ASM__
#endif  // __ASM__

#endif
