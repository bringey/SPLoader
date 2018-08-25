/*
** File: include/i386-pc/stage0a/bootstrap.h
**
** Author: bringey
**
** Description:	Error codes and locations for the MBR bootstrap
*/

#ifndef	_I386_PC_STAGE0A_BOOTSTRAP_H
#define	_I386_PC_STAGE0A_BOOTSTRAP_H

#define BOOT_SEGMENT 0x07C0        // default BIOS addr to load boot sector
#define BOOT_ADDRESS 0x7C00

#define BOOT_LBA_LOCATION 0x1B8

// error codes

#define E_BOOTSTRAP_DEFAULT   '0'
#define E_BOOTSTRAP_DISK      '1'
#define E_BOOTSTRAP_NOLBA     '2'
#define E_BOOTSTRAP_TOOBIG    '3'
#define E_BOOTSTRAP_TOOSMALL  '4'
#define E_BOOTSTRAP_BADHEADER '5'

// debug codes

#define E_BOOTSTRAP_DEBUG1   'X'
#define E_BOOTSTRAP_DEBUG2   'Y'
#define E_BOOTSTRAP_DEBUG3   'Z'

#ifndef __ASM__
#endif  // __ASM__

#endif
