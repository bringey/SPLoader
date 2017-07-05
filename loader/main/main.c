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

#include <SPLoader/console/out.h>

int main(void) {

    //c_clearscreen();
    con_clear();

    //c_puts("SPLoader v0.1\n");
    con_puts("SPLoader\n");

    memmap_dump(true);

    //con_setCursor(40, 12);

    // for (int i = 0; i != 25; ++i) {
    //     con_puts("Test Line: ");
    //     con_putchar('A' + i);
    //     con_putchar('\n');
    // }

    // con_scroll(3);

    // con_puts("test test test");
    // con_puts_at(10, 10, "STR placed at 10,10");

    // con_printf("Printf testing\n");
    // con_printf_at(20, 20, "PRINTF placed at 20,20");

    // con_printf("%-8x", 0xBABE);

    //con_puts("Test test test\n");
    //con_puts("scroll");
    //con_driver_scroll(1);

    return 0;
}