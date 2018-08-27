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
#include <version.h>


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

    con_clear();

    con_printf("SPLoader (v%s)\n", VERSION_STR);
    con_puts("Initializing: ");

    con_puts("mem ");
    mem_init();

    con_puts("disk ");
    disk_init();

    return 0;
}