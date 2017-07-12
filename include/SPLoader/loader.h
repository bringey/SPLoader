/*
** File: include/SPLoader/loader.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_LOADER_H
#define _SPLOADER_LOADER_H

//
// Location where loader_early.bin gets loaded to
//
#define LOADER16_SEGMENT 0x7C0
#define LOADER16_ADDRESS 0x200

//
// Where loader.bin gets loaded to
//
#define LOADER_ADDRESS 0x10000
//#define LOADER_STACK   0x10000
#define LOADER_SEGMENT 0x01000

//
// Location of the Loader's stack (0000:4000)
//
#define LOADER_STACK_ADDRESS 0x4000
#define LOADER_STACK_SEGMENT 0x0000

//
// Location of the loader's GDT
// 0xD00 - 0xD28
//
#define LOADER_GDT_SEGMENT 0x0D0
#define LOADER_GDT_ADDRESS 0xD00

#define LOADER_GDT_NULL   0x00
#define LOADER_GDT_CODE32 0x08
#define LOADER_GDT_DATA32 0x10
#define LOADER_GDT_CODE16 0x18
#define LOADER_GDT_DATA16 0x20

//
// Location of the loader's IDT
// 0x500 - 0xD00
//
#define LOADER_IDT_SEGMENT 0x050
#define LOADER_IDT_ADDRESS 0x500

#ifndef __ASM__
#endif  // __ASM__

#endif