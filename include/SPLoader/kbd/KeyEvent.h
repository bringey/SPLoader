/*
** File: include/SPLoader/kbd/KeyEvent.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KEYEVENT_H
#define _SPLOADER_KBD_KEYEVENT_H

#ifndef __ASM__

#include <SPLoader/kbd/Key.h>
#include <SPLoader/kbd/KeyFlags.h>


typedef struct KeyEvent_s {
    Key key;
    KeyFlags flags;
} KeyEvent;

#endif  // __ASM__

#endif