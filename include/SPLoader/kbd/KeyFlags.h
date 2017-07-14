/*
** File: include/SPLoader/kbd/KeyFlags.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_KBD_KEYFLAGS_H
#define _SPLOADER_KBD_KEYFLAGS_H

#define KEY_FLAG_PRESSED        0x01
#define KEY_FLAG_SHIFT          0x02
#define KEY_FLAG_CTRL           0x04
#define KEY_FLAG_ALT            0x08
#define KEY_FLAG_NUM_LOCK       0x10
#define KEY_FLAG_CAPS_LOCK      0x20
#define KEY_FLAG_SCROLL_LOCK    0x40


#ifndef __ASM__

#include <stdint.h>

//
// Type definition for a key flags bitmap.
//
// Bit 0: Set if the key was pressed (make)
// Bit 1: Set if the shift modifier is active
// Bit 2: Set if the ctrl modifier is active
// Bit 3: Set if the alt modifier is active
// Bit 4: Set if num lock is active
// Bit 5: Set if caps lock is active
// Bit 6: Set if scroll lock is active
// Bits 7-31: Reserved
//
typedef uint32_t KeyFlags;

#endif  // __ASM__

#endif