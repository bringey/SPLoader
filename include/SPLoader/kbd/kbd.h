/*
** File: include/SPLoader/kbd/kbd.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KBD_H
#define _SPLOADER_KBD_KBD_H

#ifndef __ASM__

#include <SPLoader/kbd/KeyEvent.h>

int spl_kbd_init(void);

int spl_kbd_waitForEvent(KeyEvent *evt);

#endif  // __ASM__

#endif