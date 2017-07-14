/*
** File: include/SPLoader/kbd/kbd.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KBD_H
#define _SPLOADER_KBD_KBD_H

#ifndef __ASM__

int kbd_init(void);

int kbd_waitForEvent(KeyEvent *evt);

#endif  // __ASM__

#endif