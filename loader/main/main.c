/*
** File: loader/main/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#include <SPLoader/i386-pc/c_io.h>
#include <SPLoader/i386-pc/mem/map.h>

#include <SPLoader/console/console.h>

int main(void) {

    //c_clearscreen();

    //c_puts("SPLoader v0.1\n");

    //memmap_dump(true);

    con_init();
    con_clear();
    //con_setCursor(40, 12);

    for (int i = 0; i != 25; ++i) {
        con_puts("Test Line: ");
        con_putchar('A' + i);
        con_putchar('\n');
    }

    con_driver_scroll(3);

    //con_puts("Test test test\n");
    //con_puts("scroll");
    //con_driver_scroll(1);

    return 0;
}