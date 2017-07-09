/*
** File: SPLoader/i386-pc/ps2/ps2.h
**
** Author: Brennan Ringey
**
** PS/2 Constants
*/

#ifndef _SPLOADER_I386_PC_PS2_PS2_H
#define _SPLOADER_I386_PC_PS2_PS2_H

//
// PS/2 I/O Ports
//
#define PS2_PORT_DATA 0x60 // data port, read/write
#define PS2_PORT_CMD  0x64 // command register, write only
#define PS2_PORT_STAT 0x64 // status register, read only

//
// PS/2 Status register
// Bits 4-5 are unknown (chipset-specific)
//
#define PS2_STAT_OUTPUT_BUFFER 0x1    // bit 0: set if output buffer is full
#define PS2_STAT_INPUT_BUFFER  0x2    // bit 1: set if input buffer is full
#define PS2_STAT_SYSTEM_FLAG   0x4    // bit 2: cleared by firmware after POST
#define PS2_STAT_CMD           0x8    // bit 3: 
#define PS2_STAT_TIMEOUT       0x40   // bit 6: set if time-out error occurred
#define PS2_STAT_PARITY        0x80   // bit 7: set if a parity error occurred

//
// PS/2 Controller Configuration
// The controller configuration is a byte read/written to using commands
// PS2_CMD_READ_RAM/PS2_CMD_WRITE_RAM. Bits 3 and 7 are zero
//
#define PS2_CONFIG_PORT1_INT   0x1    // first port interrupts enabled
#define PS2_CONFIG_PORT2_INT   0x2    // second port interrupts
#define PS2_CONFIG_SYSTEM_FLAG 0x4    // system flag, should be 1
#define PS2_CONFIG_PORT1_CLOCK 0x10   // first port clock
#define PS2_CONFIG_PORT2_CLOCK 0x20   // second port clock
#define PS2_CONFIG_PORT1_TRANS 0x40   // first port translation

//
// PS/2 Controller Output Port
// Can be read/written to using commands PS2_CMD_READ_CTRL_OUT and
// PS2_CMD_WRITE_CTRL_OUT
//
#define PS2_CTRLOUT_SYSTEM              0x1
#define PS2_CTRLOUT_A20                 0x2
#define PS2_CTRLOUT_PORT2_CLOCK         0x4
#define PS2_CTRLOUT_PORT2_DATA          0x8
#define PS2_CTRLOUT_PORT1_OUTPUT_BUFFER 0x10
#define PS2_CTRLOUT_PORT2_OUTPUT_BUFFER 0x20
#define PS2_CTRLOUT_PORT1_CLOCK         0x40
#define PS2_CTRLOUT_PORT1_DATA          0x80


//
// PS/2 Controller Commands
// Commands are written to the PS2_PORT_CMD port. Each command is at least one
// byte. Some commands require an additional byte to be written.
//
#define PS2_CMD_READ_RAM        0x20
#define PS2_CMD_WRITE_RAM       0x60

#define PS2_CMD_TEST_CTRL       0xAA
#define PS2_CMD_TEST_PORT_1     0xAB
#define PS2_CMD_TEST_PORT_2     0xA9

#define PS2_CMD_ENABLE_PORT_1   0xAE
#define PS2_CMD_ENABLE_PORT_2   0xA8

#define PS2_CMD_DISABLE_PORT_1  0xAD
#define PS2_CMD_DISABLE_PORT_2  0xA7

#define PS2_CMD_COPY_STAT_03    0xC1
#define PS2_CMD_COPY_STAT_47    0xC2
#define PS2_CMD_READ_CTRL_OUT   0xD0
#define PS2_CMD_WRITE_CTRL_OUT  0xD1
#define PS2_CMD_WRITE_PORT1_OUT 0xD2
#define PS2_CMD_WRITE_PORT2_OUT 0xD3
#define PS2_CMD_WRITE_PORT2_IN  0xD4

// Non-standard, varies per chipset

#define PS2_CMD_DIAGNOSTIC      0xAC
#define PS2_CMD_READ_CTRL_IN    0xC0

//
// Response Bytes (RB)
// Bytes sent from the keyboard/device that have special meaning
//
#define PS2_RB_ERROR1           0x00  // key-detection error, buffer overrun
#define PS2_RB_SELFTEST_PASSED  0xAA  // self-test passed after reset/power up
#define PS2_RB_ECHO             0xEE  // echo
#define PS2_RB_ACK              0xFA  // command acknowledgement
#define PS2_RB_SELFTEST_FAILED1 0xFC  // self-test failed after reset/power up
#define PS2_RB_SELFTEST_FAILED2 0xFF  // same as SELFTEST_FAILED1
#define PS2_RB_RESEND           0xFE  // resend (repeat last command sent)
#define PS2_RB_ERROR2           0xFF  // same as ERROR1

#define PS2_RB_ERROR PS2_RB_ERROR1
#define PS2_RB_SELFTEST_FAILED PS2_RB_SELFTEST_FAILED1



#endif