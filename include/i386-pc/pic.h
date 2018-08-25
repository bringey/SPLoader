/*
** File: include/i386-pc/pic.h
**
** Author: bringey
**
*/

#ifndef _I386_PC_PIC_H
#define _I386_PC_PIC_H

//
// Ports
//
#define PIC_MASTER_BASE_PORT    0x20
#define PIC_MASTER_CMD_PORT     PIC_MASTER_BASE_PORT
#define PIC_MASTER_DATA_PORT    (PIC_MASTER_BASE_PORT+1)
#define PIC_SLAVE_BASE_PORT     0xA0
#define PIC_SLAVE_CMD_PORT      PIC_SLAVE_BASE_PORT
#define PIC_SLAVE_DATA_PORT     (PIC_SLAVE_BASE_PORT+1)

#define PIC_EOI                 0x20

#define PIC_ICW1_NEEDICW4       0x01    // ICW4 needed
#define PIC_ICW1_SINGLE         0x02    // single (cascade) mode
#define PIC_ICW1_INTERVAL4      0x04    // call address interval 4
#define PIC_ICW1_LEVEL          0x08    // level triggered mode
#define PIC_ICW1_INIT           0x10    // initialization - required

#define PIC_ICW4_8086           0x01    // 8086/88 mode
#define PIC_ICW4_AUTO_EOI       0x02    // Auto EOI
#define PIC_ICW4_BUF_SLAVE      0x08    // buffered mode (slave)
#define PIC_ICW4_BUF_MASTER     0x0C    // buffered mode (master)
#define PIC_ICW4_SFN            0x10    // special fully nested mode



#ifndef __ASM__
#endif  // __ASM__

#endif