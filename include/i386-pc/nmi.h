
#ifndef _I386_PC_NMI_H
#define _I386_PC_NMI_H

#define NMI_CTRL_PORT_A 0x92
#define NMI_CTRL_PORT_B 0x61

// Control Port A layout

#define NMI_ALT_HOT_RESET 0x1   // bit 0: alternate hot reset
#define NMI_ALT_A20GATE   0x2   // bit 1: alternate a20 gate
                                // Bit 2: RESERVED
#define NMI_SECURITY      0x8   // bit 3: security lock
#define NMI_WATCHDOG      0x10  // bit 4: watchdog timer status
                                // Bit 5: RESERVED
#define NMI_HDD2          0x40  // bit 6: HDD 2 drive activity
#define NMI_HDD1          0x80  // bit 7: HDD 1 drive activity

// Control Port B layout

#define NMI_SPEAKER_TIMER        0x1  // bit 0: Timer 2 tied to speaker
#define NMI_SPEAKER              0x2  // bit 1: Speaker data enable
#define NMI_PARITY_CHECK_ENABLE  0x4  // bit 2: Parity check enable
#define NMI_CHANNEL_CHECK_ENABLE 0x8  // bit 3: Channel check enable
#define NMI_REFRESH              0x10 // bit 4: Refresh request
#define NMI_TIMER2               0x20 // bit 5: Timer 2 output
#define NMI_CHANNEL_CHECK        0x40 // bit 6: Channel check
#define NMI_PARITY_CHECK         0x80 // bit 7: Parity check




#ifndef __ASM__

void __nmi_enable(void);

void __nmi_disable(void);


#endif


#endif