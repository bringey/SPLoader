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

    c_clearscreen();

    c_puts("SPLoader v0.1\n");

    memmap_dump(true);

    con_init();
    con_clear();
    con_setCursor(40, 12);

    return 0;
}