/*
** File: loader/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#include <SPLoader/console.h>

#include <loader/disk.h>


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

    con_puts("Initializing disk driver\n");
    disk_init();
    con_printf("Block size: %d\n", disk_blockSize());
    con_printf("Disk Buffer located at 0x%08x\n", disk_buffer());


    return 0;
}