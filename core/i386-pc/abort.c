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

#include <loader/i386-pc/loader.h>

#include <stddef.h>

void _spl_abort(void) {
    spl_con_puts("ABORTED! System is halted...");
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void _spl_error(
    #ifdef DEBUG_FILENAMES
        const char *file, 
        unsigned line,
    #endif
        int errno,
        const char *msg
) {
    spl_con_puts("[ERROR] ");

    #ifndef NDEBUG
        if (errno == E_ASSERT) {
            msg = "assertion has failed";
        }
    #endif

    if (errno != E_UNSPECIFIED) {
        spl_con_printf("(%d) ", errno);
    }
    spl_con_printf("%s\n", msg);

    #ifdef DEBUG_FILENAMES
        spl_con_printf("[ERROR] at %s:%d\n", file, line);
    #endif

    _spl_abort();
}

// void _spl_abort(
//     #ifdef DEBUG_FILENAMES
//         const char *file, 
//         unsigned line,
//     #endif
//         const char *reason
// ) {
//     asm volatile ("cli");
//     spl_con_printf("ABORTED: %s\n", reason);

//     #ifdef DEBUG_FILENAMES
//     spl_con_printf("%s:%d\n", file, line);
//     #endif
    
//     #ifdef DEBUG_BACKTRACE
//     // backtrace
//     // print all return addresses on the stack
//     // ASSUMES CDECL CALLING CONVENTION

//     spl_con_puts("  %EBP     4(%EBP)  \xB3  8(%EBP) 12(%EBP) 16(%EBP) 20(%EBP) 24(%EBP) 28(%EBP)\n");

//     unsigned *ebp = (unsigned*)__ebp(); // get the current EBP value
//     #define MAX_FRAMES 20               // max number of frames to unwind
//     unsigned frames = 0;
//     unsigned eip;

//     while (frames < MAX_FRAMES && ebp < (unsigned*)LOADER_STACK_ADDRESS) {
//         eip = ebp[1]; // get the return address
//         spl_con_printf("(0x%04x) 0x%08x \xB3", ebp, eip); // print frame's return address
//         // print parameter data
//         for (unsigned i = 0; i < 6; ++i) {
//             // for routines without/with less than 6 parameters, local data of
//             // the next frame will be displayed
//             spl_con_printf(" %08x", ebp[i + 2]);
//         }

//         spl_con_putchar('\n');
//         ebp = (unsigned*)ebp[0];        // get next frame
//         ++frames;
//     }

//     #endif // DEBUG_BACKTRACE

//     for (;;) {
//         asm("hlt");
//     }
// }

