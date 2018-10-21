/*
** File: src/main.c
**
** Author: bringey
**
** Contains the main function for the loader
**
*/

#include <loader/console.h>

#include <loader/abort.h>
#include <loader/disk.h>
#include <loader/mem.h>
#include <loader/version.h>

#include <loader/disk/gpt.h>

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

    con_clear();

    con_printf("SPLoader (v%s)\n", VERSION_STR);

    // initialization routines

    con_puts("Initialize memory\n");
    mem_init();

    con_puts("Initialize disk\n");
    disk_init();

    // get the disk we booted from (aka: boot disk)
    Disk disk;
    disk_bootDisk(&disk);

    // detect the disk label
    DiskLabel label = disk_detect(&disk);
    // find the boot partition
    DiskPart bootpart;
    disk_bootPart(&disk, label, &bootpart);

    // mount it

    // load config file

    // do the loading


    return 0;
}