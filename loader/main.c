/*
** File: loader/main.c
**
** Author: bringey
**
** Contains the main function for the loader
**
*/

#include <loader/console.h>

#include <loader/abort.h>
#include <loader/err.h>
#include <loader/disk.h>
#include <loader/mem.h>

#include <sploader.h>

//
// Stage 1 main routine
// 1. initialization (mem, disk, vfs)
// 3. Detect disk label (MBR, GPT, etc)
// 4. Find boot partition on label
// 5. Mount filesystem on bootpart to /
// 6. load config (/sploader.ini)
// 7. execute config (chainload, load, etc)
//
int ldr_main(SplHeader *header, void* entryAddr) {

    ldr_con_clear();

    // verify header checksum
    if (!spl_hdr_check(header)) {
        ldr_exceptv(EX_HEADER, E_HEADER_INTEGRITY);
    }

    // verify loader.bin checksum
    if (!spl_hdr_checkBin(header, entryAddr)) {
        ldr_exceptv(EX_HEADER, E_HEADER_LOADER_INTEGRITY);
    }

    ldr_con_printf("SPLoader (v%s)\n", header->version);

    // initialization routines

    ldr_con_puts("Initialize memory\n");
    ldr_mem_init();

    ldr_con_puts("Initialize disk\n");
    ldr_disk_init();

    // get the disk we booted from (aka: boot disk)
    Disk disk;
    ldr_disk_bootDisk(&disk);

    // get the disk label from the header and verify it
    DiskLabel label;
    ldr_disk_label_init(&disk, header->label, &label);

    // get the boot partition
    DiskPart bootpart;
    if (header->flags & SPL_HEADER_FLAG_ACTIVE) {
        ldr_disk_label_getActive(&label, &bootpart);
    } else {
        ldr_disk_label_getPart(&label, header->partition, &bootpart);
    }

    // mount it

    // load config file

    // do the loading


    return 0;
}