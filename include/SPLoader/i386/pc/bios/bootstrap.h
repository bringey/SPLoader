/*
** File: include/SPLoader/i386/pc/bios/bootstrap.h
**
** Author: Brennan Ringey
**
** Description:	Error codes and locations for the MBR bootstrap
*/

#ifndef	_SPLOADER_I386_PC_BIOS_BOOTSTRAP_H
#define	_SPLOADER_I386_PC_BIOS_BOOTSTRAP_H

#define BOOT_SEGMENT 0x07C0        // default BIOS addr to load boot sector
#define BOOT_ADDRESS 0x7C00
#define BOOT_STACK   0x4000        // stack top: 07C0:4000 (0xBC00)

#define LOADER_LBA_LOCATION    0x1B4 // location of the loader LBA
#define LOADER_SIZE_LOCATION   0x1B8 // location of the loader size
#define LOADER16_SIZE_LOCATION 0x1BA // location of the loader16 size

#define mkcode(type, char) (type << 8) | char

// error types

#define ET_BOOTSTRAP_BOOT  0
#define ET_BOOTSTRAP_DEBUG 1


// error codes
//   byte 1: 0
//   byte 0: error code character

E_BOOTSTRAP_DEFAULT  = mkcode(ET_BOOTSTRAP_BOOT, '0')
E_BOOTSTRAP_DISK     = mkcode(ET_BOOTSTRAP_BOOT, '1')
E_BOOTSTRAP_NOLBA    = mkcode(ET_BOOTSTRAP_BOOT, '2')
E_BOOTSTRAP_TOOBIG   = mkcode(ET_BOOTSTRAP_BOOT, '3')
E_BOOTSTRAP_TOOSMALL = mkcode(ET_BOOTSTRAP_BOOT, '4')

// debug codes
//   byte 1: 1
//   byte 0: debug code character

E_BOOTSTRAP_DEBUG1 = mkcode(ET_BOOTSTRAP_DEBUG, 'A')
E_BOOTSTRAP_DEBUG2 = mkcode(ET_BOOTSTRAP_DEBUG, 'B')
E_BOOTSTRAP_DEBUG3 = mkcode(ET_BOOTSTRAP_DEBUG, 'C')


#endif
