/*
** File: src/main.c
**
** Author: bringey
**
** Contains the main function for the loader
**
*/

#include <console.h>

#include <abort.h>
#include <disk.h>
#include <mem.h>
#include <version.h>

#include <disk/gpt.h>

//
// Stage 1 main routine
// 1. initialization (mem, disk, vfs)
// 3. Detect disk label (MBR, GPT, etc)
// 4. Find boot partition on label
// 5. Mount filesystem on bootpart to /
// 6. load config (/sploader.ini)
// 7. execute config (chainload, load, etc)
//
int main(void) {
    Disk disk;

    con_clear();

    con_printf("SPLoader (v%s)\n", VERSION_STR);

    // initialization routines

    con_puts("Initialize memory\n");
    mem_init();

    con_puts("Initialize disk\n");
    disk_init(&disk);

    // detect the disk label
    DiskLabel label = disk_detect();
    // find the boot partition
    DiskPart bootpart;
    disk_findBoot(label, &bootpart);

    // mount it

    // load config file

    // do the loading


    return 0;
}