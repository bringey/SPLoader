
#include <SPLoader/i386-pc/c_io.h>
#include <SPLoader/i386-pc/mem/map.h>

int main(void) {

    c_clearscreen();

    c_puts("SPLoader v0.1\n");

    memmap_dump(true);

    return 0;
}