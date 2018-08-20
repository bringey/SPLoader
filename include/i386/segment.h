/*
** File: include/SPLoader/i386/segment.h
**
** Author: Brennan Ringey
**
**
*/

#ifndef _SPLOADER_I386_SEGMENT_H
#define _SPLOADER_I386_SEGMENT_H

/*
** Memory management
*/
#define SEG_PRESENT             0x80
#define SEG_PL_0                0x00
#define SEG_PL_1                0x20
#define SEG_PL_2                0x40
#define SEG_PL_3                0x50
#define SEG_SYSTEM              0x00
#define SEG_NON_SYSTEM          0x10
#define SEG_32BIT               0x04
#define DESC_IGATE              0x06

/*
** PMode segment selectors
**
** IA-32 V3, page 3-8.
*/
#define SEG_SEL_IX              0xfff8
#define SEG_SEL_TI              0x0004
#define SEG_SEL_RPL             0x0003

/*
** Segment descriptor bytes
**
** IA-32 V3, page 3-10.
*/
    /* byte 5 - access control */
#define SEG_ACCESS_TYPE_MASK    0x0f
#define SEG_DATA_A_BIT          0x1
#define SEG_DATA_W_BIT          0x2
#define SEG_DATA_E_BIT          0x4
#define SEG_CODE_A_BIT          0x1
#define SEG_CODE_R_BIT          0x2
#define SEG_CODE_C_BIT          0x4
#define SEG_DATA_RO             0x0
#define SEG_DATA_ROA            0x1
#define SEG_DATA_RW             0x2
#define SEG_DATA_RWA            0x3
#define SEG_DATA_RO_XD          0x4
#define SEG_DATA_RO_XDA         0x5
#define SEG_DATA_RW_XW          0x6
#define SEG_DATA_RW_XWA         0x7
#define SEG_CODE_XO             0x8
#define SEG_CODE_XOA            0x9
#define SEG_CODE_XR             0xa
#define SEG_CODE_XRA            0xb
#define SEG_CODE_XO_C           0xc
#define SEG_CODE_XO_CA          0xd
#define SEG_CODE_XR_C           0xe
#define SEG_CODE_XR_CA          0xf

#define SEG_ACCESS_S_BIT        0x10
#define SEG_S_SYSTEM            0x00
#define SEG_S_NON_SYSTEM        0x10

#define SEG_ACCESS_DPL_MASK     0x60
#define SEG_DPL_0               0x00
#define SEG_DPL_1               0x20
#define SEG_DPL_2               0x40
#define SEG_DPL_3               0x60

#define SEG_ACCESS_P_BIT        0x80

    /* byte 6 - sizes */
#define SEG_SIZE_LIM_19_16      0x0f

#define SEG_SIZE_AVL_BIT        0x10

#define SEG_SIZE_D_B_BIT        0x40
#define SEG_DB_16BIT            0x00
#define SEG_DB_32BIT            0x40

#define SEG_SIZE_G_BIT          0x80
#define SEG_GRAN_BYTE           0x00
#define SEG_GRAN_4KBYTE         0x80

/*
** System-segment and gate-descriptor types
**
** IA-32 V3, page 3-15.
*/
    /* type 0 reserved */
#define SEG_SYS_16BIT_TSS_AVAIL 0x1
#define SEG_SYS_LDT             0x2
#define SEG_SYS_16BIT_TSS_BUSY  0x3
#define SEG_SYS_16BIT_CALL_GATE 0x4
#define SEG_SYS_TASK_GATE       0x5
#define SEG_SYS_16BIT_INT_GATE  0x6
#define SEG_SYS_16BIT_TRAP_GATE 0x7
    /* type 8 reserved */
#define SEG_SYS_32BIT_TSS_AVAIL 0x9
    /* type A reserved */
#define SEG_SYS_32BIT_TSS_BUSY  0xb
#define SEG_SYS_32BIT_CALL_GATE 0xc
    /* type D reserved */
#define SEG_SYS_32BIT_INT_GATE  0xe
#define SEG_SYS_32BIT_TRAP_GATE 0xf


#ifndef __ASM__
#endif  // __ASM__


#endif
