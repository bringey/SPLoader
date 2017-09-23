/*
** File: include/SPLoader/kbd/kbd.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KBD_H
#define _SPLOADER_KBD_KBD_H

#error "DEPRECATED! do not use"

#ifndef __ASM__

#include <SPLoader/kbd/KeyEvent.h>

int kbd_init(void);

int kbd_waitForEvent(KeyEvent *evt);

#endif  // __ASM__

#endif