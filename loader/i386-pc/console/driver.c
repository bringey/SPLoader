/*
** File: loader/i386-pc/console/driver.c
**
** Author: Brennan Ringey
**
** i386-pc console driver
**
** Contains implementations for functions defined in SPLoader/console/driver.h
** for the i386-pc platform.
**
*/

#include <SPLoader/console/driver.h>
#include <SPLoader/err.h>

#include <stddef.h>

#include "constants.h"

//
// con_driver_clear is implemented in assembly, see _driver.S
//

int con_driver_height(unsigned *heightVar) {
    if (heightVar == NULL) {
        return E_ARGNULL;
    }

    *heightVar = VGA_HEIGHT;

    return E_SUCCESS;
}

int con_driver_put(char ch, unsigned x, unsigned y) {
    (void)ch; (void)x; (void)y;
    return -1;
}

int con_driver_scroll(unsigned lines) {
    (void)lines;
    return -1;
}

int con_driver_updateCursor(unsigned x, unsigned y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return E_ARGBOUNDS;
    }

    unsigned cursor = (y * VGA_WIDTH) + x;

    //
    // The following sets the cursor position using inline assembly
    // Since this function is called often, we avoid calling the support
    // function __outb 4 times per call to this function to speed things up.
    //
    // Should this be proven unnecessary, the inline assembly can be commented
    // out, and the __outb calls can be uncommented
    //

    //__outb(0x3D4, 0xE);
    __asm("movb $0xE, %%al; \
           movw $0x3D4, %%dx; \
           outb (%%dx);"
          :
          :
          :"%eax","%edx");

    //__outb(0x3D5, (cursor >> 8) & 0xFF);
    __asm("movl %0, %%eax; \
           shrw $8, %%ax; \
           movw $0x3D5, %%dx; \
           outb (%%dx);"
          :
          :"r"(cursor)
          :"%eax","%edx");

    //__outb(0x3D4, 0xF);
    __asm("movb $0xF, %%al; \
           movw $0x3D4, %%dx; \
           outb (%%dx);"
          :
          :
          :"%eax","%edx");

    // __outb(0x3D5, cursor & 0xFF);
    __asm("movl %0, %%eax; \
           movw $0x3D5, %%dx; \
           outb (%%dx);"
          :
          :"r"(cursor)
          :"%eax","%edx");

    return E_SUCCESS;
}

int con_driver_width(unsigned *widthVar) {
    if (widthVar == NULL) {
        return E_ARGNULL;
    }

    *widthVar = VGA_WIDTH;

    return E_SUCCESS;
}