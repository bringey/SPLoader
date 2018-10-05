/*
** File: include/i386-pc/loader.h
**
** Author: bringey
**
** Contains memory locations (for i386 systems) for various things used by the
** loader. Note that all locations defined here exist in low memory (<0x100000)
**
** Define any fixed memory locations for modules here, this way things can
** stay organized and there will less chance of conflicts.
*/

#ifndef _I386_PC_LOADER_H
#define _I386_PC_LOADER_H

#include <i386-pc/stage0a/bootstrap.h>

#define flataddr(seg, off) ((seg << 4) + off)

// UNUSABLE ----------------------------------------------- 0x000000 - 0x0004FF

// IDT ---------------------------------------------------- 0x000500 - 0x000CFF

#define LOADER_IDT_ADDRESS          0x500

// GDT ---------------------------------------------------- 0x000D00 - 0x000D27

// Location of the loader's GDT, as well as entry offsets
// See src/i386-pc/stage0b/early.S for how this is setup

#define LOADER_GDT_ADDRESS          0xD00

#define LOADER_GDT_NULL             0x00
#define LOADER_GDT_CODE32           0x08
#define LOADER_GDT_DATA32           0x10
#define LOADER_GDT_CODE16           0x18
#define LOADER_GDT_DATA16           0x20

// STACK -------------------------------------------------- 0x000D28 - 0x003FFF

// Program stack used by all stages, starts at 0x4000 and grows downward

#define LOADER_STACK_ADDRESS        0x4000

// MMAP --------------------------------------------------- 0x004000 - 0x0043FF

// Memory map, acquired in stage 0b using BIOS E820

// memory map
#define LOADER_MMAP_LENGTH          0x4000
#define LOADER_MMAP_ENTRY_SIZE      0x4004
#define LOADER_MMAP_TABLE           0x4008

// AVAILABLE ---------------------------------------------- 0x004400 - 0x004FFF

// REALMODE ----------------------------------------------- 0x005000 - 0x0051FF

// location of the realmode utility program (aka __int32)
#define LOADER_REALMODE_ADDRESS     0x5000

// AVAILABLE ---------------------------------------------- 0x005200 - 0x007BFF

// MBR ---------------------------------------------------- 0x007C00 - 0x007DFF

#define LOADER_MBR_ADDRESS          0x7C00

// BOOT_HEADER -------------------------------------------- 0x007E00 - 0x007FFF

#define LOADER_BOOT_HEADER_ADDRESS  flataddr(BOOT_SEGMENT, BOOT_BOOTHEADER)

// DAP, DP ------------------------------------------------ 0x008000 - 0x0081FF

#define LOADER_DP_ADDRESS           flataddr(BOOT_SEGMENT, BOOT_DP)
#define LOADER_DAP_ADDRESS          flataddr(BOOT_SEGMENT, BOOT_DAP)

// AVAILABLE ---------------------------------------------- 0x008200 - 0x00FFFF

// LOADER.BIN --------------------------------------------- 0x010000 - 0x01FFFF

//
// Where loader.bin gets loaded to
//
#define LOADER_ADDRESS              0x10000
#define LOADER_SEGMENT              0x01000

// DD ----------------------------------------------------- 0x020000 - 0x02FFFF

// DD: Disk driver
#define LOADER_DD_BUFFER            0x20000
// buffer location segment:offset = 2000:0000
#define LOADER_DD_SEGMENT           0x02000
#define LOADER_DD_OFFSET            0x00000



#ifndef __ASM__
#endif  // __ASM__

#endif