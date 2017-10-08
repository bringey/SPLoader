/*
** File: core/i386-pc/abort.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/abort.h>
#include <SPLoader/err.h>
#include <SPLoader/console.h>

#include <SPLoader/i386/regs.h>
#include <SPLoader/i386-pc/loader.h>

#include <stddef.h>

void _abort(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        const char *reason
) {
    asm volatile ("cli");
    con_printf("ABORTED: %s\n", reason);

    #ifdef DEBUG_FILENAMES
    con_printf("%s:%d\n", file, line);
    #endif
    
    #ifdef DEBUG_BACKTRACE
    // backtrace
    // print all return addresses on the stack
    // ASSUMES CDECL CALLING CONVENTION

    con_puts("  %EBP     4(%EBP)  \xB3  8(%EBP) 12(%EBP) 16(%EBP) 20(%EBP) 24(%EBP) 28(%EBP)\n");

    unsigned *ebp = (unsigned*)__ebp(); // get the current EBP value
    #define MAX_FRAMES 20               // max number of frames to unwind
    unsigned frames = 0;
    unsigned eip;

    while (frames < MAX_FRAMES && ebp < (unsigned*)LOADER_STACK_ADDRESS) {
        eip = ebp[1]; // get the return address
        con_printf("(0x%04x) 0x%08x \xB3", ebp, eip); // print frame's return address
        // print parameter data
        for (unsigned i = 0; i < 6; ++i) {
            // for routines without/with less than 6 parameters, local data of
            // the next frame will be displayed
            con_printf(" %08x", ebp[i + 2]);
        }

        con_putchar('\n');
        ebp = (unsigned*)ebp[0];        // get next frame
        ++frames;
    }

    #endif // DEBUG_BACKTRACE

    for (;;) {
        asm("hlt");
    }
}

