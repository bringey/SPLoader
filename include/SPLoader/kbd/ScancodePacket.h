
#ifndef _SCANCODEPACKET_H
#define _SCANCODEPACKET_H

#define SCANCODE_PACKET_BUFSIZE 16

#ifndef __ASM__

#include <stdint.h>

typedef struct ScancodePacket_s {

    uint8_t length;
    uint8_t buffer[SCANCODE_PACKET_BUFSIZE];

} ScancodePacket;

#endif  // __ASM__

#endif