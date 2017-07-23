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
#include <stdint.h>

#include "constants.h"


#define calcIndex(col, row) ((row * VGA_WIDTH) + col)

static uint16_t curColor = VGA_DEFAULT_COLOR;


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
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return E_ARGBOUNDS;
    }

    unsigned cursor = calcIndex(x, y);

    ((uint16_t*)(VGA_BUFFER))[cursor] = (uint16_t)(curColor | ((unsigned char)ch));

    return E_SUCCESS;
}

int con_driver_scroll(unsigned lines) {

    // if lines is zero, we don't need to do anything

    if (lines) {

        // if the lines is greater than the height, just clear it
        // otherwise, copy it line by line
        if (lines >= VGA_HEIGHT) {
            con_driver_clear();
        } else {

            // size, in cells, of the scrolling region
            unsigned scrollLength = VGA_WIDTH * (VGA_HEIGHT - lines);
            // size, in cells, of the blank region (empty rows created after
            // the scroll)
            unsigned windowLength = VGA_WIDTH * lines;

            // shift the scroll region via a 'rep movsw'
            // %esi = VGA_BUFFER + windowLength
            // %edi = VGA_BUFFER
            // %ecx = scrollLength

            asm volatile (
                "movl   %0, %%esi;"
                "movl   %1, %%edi;"
                "movl   %2, %%ecx;"
                "cld;"
                "rep movsw;"
                :
                : "imr"(((uint16_t*)VGA_BUFFER) + windowLength),
                  "imr"(VGA_BUFFER),
                  "imr"(scrollLength)
                : "%esi", "%edi", "ecx"
            );

            // clear the blank region via a 'rep stosw'
            // %eax = VGA_NULLCELL
            // %edi = VGA_BUFFER + scrollLength
            // %ecx = windowLength

            asm volatile (
                "movl   %0, %%eax;"
                "movl   %1, %%edi;"
                "movl   %2, %%ecx;"
                "cld;"
                "rep stosw;"
                :
                : "imr"(VGA_NULLCELL),
                  "imr"(((uint16_t*)VGA_BUFFER) + scrollLength),
                  "imr"(windowLength)
                : "%eax", "%edi", "%ecx"
            );

        }

    }

    return E_SUCCESS;
}

int con_driver_setBgColor(ConColor color) {
    color = (color & 0xF) << 12;
    curColor = (curColor & 0x0F00) | color;
    return E_SUCCESS;
}

int con_driver_setFgColor(ConColor color) {
    color = (color & 0xF) << 8;
    curColor = (curColor & 0xF000) | color;
    return E_SUCCESS;
}

int con_driver_updateCursor(unsigned x, unsigned y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
        return E_ARGBOUNDS;
    }

    unsigned cursor = calcIndex(x, y);

    //
    // The following sets the cursor position using inline assembly
    // Since this function is called often, we avoid calling the support
    // function __outb 4 times per call to this function to speed things up.
    //
    // Should this be proven unnecessary, the inline assembly can be commented
    // out, and the __outb calls can be uncommented
    //

    //__outb(0x3D4, 0xE);
    asm volatile (
        "movb $0xE, %%al;"
        "movw $0x3D4, %%dx;"
        "outb (%%dx);"
        :
        :
        : "%eax","%edx"
    );

    //__outb(0x3D5, (cursor >> 8) & 0xFF);
    asm volatile (
        "movl %0, %%eax;"
        "shrw $8, %%ax;"
        "movw $0x3D5, %%dx;"
        "outb (%%dx);"
        :
        : "mor"(cursor)
        : "%eax","%edx"
    );

    //__outb(0x3D4, 0xF);
    asm volatile (
        "movb $0xF, %%al;"
        "movw $0x3D4, %%dx;"
        "outb (%%dx);"
        :
        :
        : "%eax","%edx"
    );

    // __outb(0x3D5, cursor & 0xFF);
    asm volatile (
        "movl %0, %%eax;"
        "movw $0x3D5, %%dx;"
        "outb (%%dx);"
        :
        : "mor"(cursor)
        : "%eax","%edx"
    );

    return E_SUCCESS;
}

int con_driver_width(unsigned *widthVar) {
    if (widthVar == NULL) {
        return E_ARGNULL;
    }

    *widthVar = VGA_WIDTH;

    return E_SUCCESS;
}