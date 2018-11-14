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
int main(SplHeader *header, void* entryAddr) {
    int err;

    con_clear();

    // verify header checksum
    if (!spl_check(header)) {
        exceptv(EX_HEADER, E_HEADER_INTEGRITY);
    }

    // verify loader.bin checksum
    if (!spl_checkBin(header, entryAddr)) {
        exceptv(EX_HEADER, E_HEADER_LOADER_INTEGRITY);
    }

    con_printf("SPLoader (v%s)\n", header->version);

    // initialization routines

    con_puts("Initialize memory\n");
    mem_init();

    con_puts("Initialize disk\n");
    disk_init();

    // get the disk we booted from (aka: boot disk)
    Disk disk;
    disk_bootDisk(&disk);

    // get the disk label from the header and verify it
    DiskLabel label;
    disk_label_init(&disk, header->label, &label);

    // check it
    disk_label_check(&label);

    // determine index of boot partition
    uint32_t bootindex;
    if (header->flags & SPL_HEADER_FLAG_ACTIVE) {
        // search for the active partition
        err = label.getActive(&label, &bootindex);
        ERRCHECK(EX_DISK_LABEL, err);
    } else {
        // use the partition index set in header
        bootindex = header->partition;
    }

    DiskPart bootpart;
    err = label.getPart(&label, bootindex, &bootpart);
    if (err == E_DISK_LABEL_NO_PARTITION) {
        except(EX_DISK_PARTITION);
    } else if (err != E_SUCCESS) {
        exceptv(EX_DISK_LABEL, err);
    }

    // mount it

    // load config file

    // do the loading


    return 0;
}