/*
** File: src/core/i386-pc/console.c
**
** Author: Brennan Ringey
**
** i386-pc console driver
**
** Contains implementations for functions defined in SPLoader/console/driver.h
** for the i386-pc platform.
**
*/

#include <SPLoader/console.h>
#include <SPLoader/err.h>

#include <stddef.h>
#include <stdint.h>


#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// no blink, black background, white foreground
#define VGA_DEFAULT_COLOR (0xF << 8)

// black background, gray foreground, ASCII space
#define VGA_NULLCELL (VGA_DEFAULT_COLOR | ' ')


#define calcIndex(col, row) ((row * VGA_WIDTH) + col)


int _spl_con_clearRegion(unsigned rowStart, unsigned rowEnd) {
    if (rowEnd <= rowStart) {
        return E_ARGBOUNDS;
    }

    // rep stosw
    // %eax: VGA_NULLCELL
    // %ecx: (rowEnd - rowStart) * VGA_WIDTH
    // %edi: VGA_BUFFER + (rowStart * VGA_WIDTH)

    asm volatile(
        "movl   %0, %%eax;"
        "movl   %1, %%ecx;"
        "movl   %2, %%edi;"
        "cld;"
        "rep stosw;"
        :
        : "i"(VGA_NULLCELL),
          "mr"((rowEnd - rowStart) * VGA_WIDTH),
          "mr"(VGA_BUFFER + (rowStart * VGA_WIDTH))
        : "%eax", "%ecx", "%edi"
    );

    return E_SUCCESS;
}


unsigned _spl_con_color(ConColor fg, ConColor bg) {
    return (unsigned)(((uint16_t)bg << 12) | ((uint16_t)fg << 8));
}

unsigned _spl_con_index(unsigned x, unsigned y) {
    return (y * VGA_WIDTH) + x;
}

int _spl_con_put(unsigned index, unsigned color, char ch) {
    if (index >= (VGA_WIDTH * VGA_HEIGHT)) {
        return E_ARGBOUNDS;
    }

    ((uint16_t*)(VGA_BUFFER))[index] = (uint16_t)(color | ((unsigned char)ch));

    return E_SUCCESS;
}

int _spl_con_scroll(unsigned lineStart, unsigned lineEnd, unsigned lines) {

    // if lines is zero, we don't need to do anything

    if (lines) {

        unsigned windowHeight = lineEnd - lineStart;
        // if the lines is greater than the height, just clear it
        // otherwise, copy it line by line
        if (lines >= windowHeight) {
            _spl_con_clearRegion(lineStart, lineEnd);
        } else {

            uint16_t *windowStart = (uint16_t*)VGA_BUFFER + (lineStart * VGA_WIDTH);

            // size, in cells, of the scrolling region
            unsigned scrollLength = VGA_WIDTH * (windowHeight - lines);
            // size, in cells, of the blank region (empty rows created after
            // the scroll)
            unsigned windowLength = VGA_WIDTH * lines;

            // shift the scroll region via a 'rep movsw'
            // %esi = windowStart + windowLength
            // %edi = windowStart
            // %ecx = scrollLength

            asm volatile (
                "movl   %0, %%esi;"
                "movl   %1, %%edi;"
                "movl   %2, %%ecx;"
                "cld;"
                "rep movsw;"
                :
                : "imr"(windowStart + windowLength),
                  "imr"(windowStart),
                  "imr"(scrollLength)
                : "%esi", "%edi", "ecx"
            );

            // clear the blank region via a 'rep stosw'
            // %eax = VGA_NULLCELL
            // %edi = windowStart + scrollLength
            // %ecx = windowLength

            asm volatile (
                "movl   %0, %%eax;"
                "movl   %1, %%edi;"
                "movl   %2, %%ecx;"
                "cld;"
                "rep stosw;"
                :
                : "imr"(VGA_NULLCELL),
                  "imr"(windowStart + scrollLength),
                  "imr"(windowLength)
                : "%eax", "%edi", "%ecx"
            );

        }

    }

    return E_SUCCESS;
}

// int con_driver_setBgColor(ConColor color) {
//     color = (color & 0xF) << 12;
//     curColor = (curColor & 0x0F00) | color;
//     return E_SUCCESS;
// }

// int con_driver_setFgColor(ConColor color) {
//     color = (color & 0xF) << 8;
//     curColor = (curColor & 0xF000) | color;
//     return E_SUCCESS;
// }

int _spl_con_updateCursor(unsigned index) {

    //unsigned cursor = calcIndex(x, y);

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
        : "mor"(index)
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
        : "mor"(index)
        : "%eax","%edx"
    );

    return E_SUCCESS;
}

// properties

unsigned _spl_con_width(void) {
    return VGA_WIDTH;
}

unsigned _spl_con_height(void) {
    return VGA_HEIGHT;
}
