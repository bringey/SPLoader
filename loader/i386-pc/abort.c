/*
** File: loader/i386-pc/abort.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/abort.h>
#include <SPLoader/err.h>
#include <SPLoader/loader.h>
#include <SPLoader/console/out.h>

#include <SPLoader/i386/regs.h>

#ifdef DEBUG_SYMBOLS
//
// Get the symbol name and location from a base address
// 0 is returned on success, failure otherise
//
static int getSymbol(unsigned addr, char **name, unsigned *location) {
    (void)addr; (void)name; (void)location;
    // TODO
    return E_FAILURE;
}

#endif // DEBUG_SYMBOLS

void _abort(
    #ifdef DEBUG_SYMBOLS
        const char *file, 
        unsigned line,
    #endif
        const char *reason
) {
    asm volatile ("cli");
    con_printf("ABORTED: %s\n", reason);

    #ifdef DEBUG_SYMBOLS

    con_printf("%s:%d\n", file, line);
    // backtrace
    // print all return addresses on the stack
    unsigned *ebp = (unsigned*)__ebp();
    ebp = (unsigned*)ebp[0]; // skip _abort's frame

    #define MAX_FRAMES 20
    unsigned frames = 0;

    unsigned eip;
    char *symbol;
    unsigned location;

    while (frames < MAX_FRAMES && ebp < (unsigned*)LOADER_STACK_ADDRESS) {
        // TODO: get function names from mapping file
        // trace syntax:
        // (frame address) return address <symbol+offset>
        eip = ebp[1];
        con_printf("(0x%04x) 0x%08x", ebp, eip); // print frame's return address
        if (getSymbol(eip, &symbol, &location) == E_SUCCESS) {
            con_printf(" <%s+0x%x>", symbol, eip - location);
        }
        con_putchar('\n');
        ebp = (unsigned*)ebp[0];        // get next frame
        ++frames;
    }

    #endif // DEBUG_SYMBOLS

    for (;;) {
        asm("hlt");
    }
}

