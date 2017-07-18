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

#include <SPLoader/sym/symbols.h>

//
// Get the symbol name and location from a base address
// 0 is returned on success, failure otherise
//
int getSymbol(unsigned address, Symbol *symbolVar) {

    unsigned length = SYMBOL_TABLE.length;

    unsigned index = 0;
    unsigned min = 0xFFFFFFFF;
    Symbol symbol;

    for (unsigned i = 0; i < length; ++i) {
        symbol = SYMBOL_TABLE.table[i];
        if (address >= symbol.location) {
            min = symbol.location;
            index = i;
        } else {
            break;
        }
    
    }

    if (min != 0xFFFFFFFF) {
        *symbolVar = SYMBOL_TABLE.table[index];
        return E_SUCCESS;
    } else {
        return E_FAILURE;
    }

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
    //ebp = (unsigned*)ebp[0]; // skip _abort's frame

    #define MAX_FRAMES 20
    unsigned frames = 0;

    unsigned eip;
    Symbol symbol;
    //char *symbol;
    //unsigned location;

    while (frames < MAX_FRAMES && ebp < (unsigned*)LOADER_STACK_ADDRESS) {
        // trace syntax:
        // (frame address) return address <symbol+offset>
        eip = ebp[1];
        con_printf("(0x%04x) 0x%08x", ebp, eip); // print frame's return address
        if (getSymbol(eip, &symbol) == E_SUCCESS) {
            con_printf(" <%s+0x%x>", symbol.name, eip - symbol.location);
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

