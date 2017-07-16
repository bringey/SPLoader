
#ifndef _SPLOADER_KBD_SCANCODEPARSEFN_H
#define _SPLOADER_KBD_SCANCODEPARSEFN_H


#ifndef __ASM__

#include <SPLoader/kbd/ScancodePacket.h>
#include <SPLoader/kbd/KeyEvent.h>

typedef int (*ScancodeParseFn)(ScancodePacket *packet, KeyEvent *evt);

#endif  // __ASM__

#endif