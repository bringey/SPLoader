/*
** File: src/main.c
**
** Author: bringey
**
** Contains the main function for the loader
**
*/

#include <console.h>

#include <disk.h>
#include <mem.h>


//
// Stage 1 main routine
// 1. initialize system disk driver (for i386-pc this is the BIOS disk driver)
// 2. initialize embedded fs driver (default is FAT32)
// 3. Find boot partition
// 4. mount boot partition as /
// 5. read /sploader/core.elf
// 6. load core.elf
// 7. transfer control to core.elf's main
//
int main(void) {

    spl_con_clear();

    // replace v0.1.0 with proper version string in header
    spl_con_printf("SPLoader (%s)\n", "v0.1.0");
    spl_con_puts("Initializing: ");

    spl_con_puts("mem ");
    spl_mem_init();

    spl_con_puts("disk ");
    disk_init();

    spl_con_putchar('\n');
    spl_mem_dump();

    return 0;
}