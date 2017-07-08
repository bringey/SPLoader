
#include <SPLoader/abort.h>
#include <SPLoader/console/out.h>


void abort(const char *reason) {
    asm volatile ("cli");
    con_printf("ABORTED: %s\n", reason);
    // TODO: add a traceback, dump registers etc
    for (;;) {
        asm("hlt");
    }
}