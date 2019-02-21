/*
** File: tools/src/install.c
**
** Author: bringey
**
** Utility for installing the bootstrap and stage 1 binary to a target.
**
*/


#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sploader.h>
#include <version.h>

#include <loader/disk/mbr.h>
#include <loader/i386-pc/stage0a/bootstrap.h>

#define USAGE_STR "spl-install <-t|--target target> <device>"
#include "common.h"

#define EXIT_DEVICE_FAILED 3
#define EXIT_BOOTSTRAP_FAILED 4
#define EXIT_LOADER_FAILED 5


struct ProgData {
    FILE *in;
    FILE *device;
    uint8_t *loaderBinBuf;
    char *devicePath;
    char *loaderBin;
    char *bootstrapBin;
    bool verbose;
};

int init(Prog *prog) {
    nullcheck(prog);
    ProgData *data = (ProgData*)malloc(sizeof(ProgData));
    if (data == NULL) {
        return EXIT_FAILURE;
    } else {
        *data = (ProgData){
            .in = NULL,
            .device = NULL,
            .devicePath = NULL,
            .loaderBinBuf = NULL,
            .loaderBin = NULL,
            .bootstrapBin = NULL,
            .verbose = false
        };
        prog->data = data;
        return EXIT_SUCCESS;
    }
}

void cleanup(Prog *prog) {
    ProgData *data = prog->data;
    if (data != NULL) {
        if (data->in != NULL) {
            fclose(data->in);
            data->in = NULL;
        }
        if (data->device != NULL) {
            fclose(data->device);
            data->device = NULL;
        }
        if (data->loaderBinBuf != NULL) {
            free(data->loaderBinBuf);
            data->loaderBinBuf = NULL;
        }
        free(data);
        prog->data = NULL;
    }
}

int parse_args(Prog *prog, int argc, char *argv[]) {

    static struct option longOpts[] = {
        {"bootstrap", required_argument, NULL, 'b'},
        {"loader", required_argument, NULL, 'l'},
        {"verbose", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    ProgData *data = prog->data;
    nullcheck(data);

    bool badoptions = false;

    int c;
    int longOptIndex = 0;
    extern int optind, optopt;
    extern char *optarg;

    opterr = 1;
    for (;;) {
        c = getopt_long(argc, argv, "b:l:", longOpts, &longOptIndex);
        if (c == EOF) {
            break;
        }

        switch (c) {
            case 0:
                break;
            case 'b':
                data->bootstrapBin = optarg;
                break;
            case 'l':
                data->loaderBin = optarg;
                break;
            case 'v':
                data->verbose = true;
                break;
            case '?':
            default:
                usage();
                return EXIT_BAD_OPTIONS;
        }
    }

    if (data->bootstrapBin == NULL) {
        eputs(prog, "no bootstrap found");
        badoptions = true;
    }

    if (data->loaderBin == NULL) {
        eputs(prog, "no loader binary found");
        badoptions = true;
    }

    if (argc - optind != 1) {
        eputs(prog, "missing installation device");
        badoptions = true;
    }

    
    if (badoptions) {
        usage();
        return EXIT_BAD_OPTIONS;
    } else {
        data->devicePath = argv[optind];
        return EXIT_SUCCESS;
    }
}

int run(Prog *prog) {

    nullcheck(prog);
    ProgData *data = prog->data;
    nullcheck(data);

    // INSTALLATION PROCEDURE
    // detect the disk label on the target device
    // open the loader binary and check for
    //  1. Correct architecture
    //  2. Version
    //  3. integrity
    // install the bootstrap (if needed)
    // patch loader header + checksum
    // install the loader


    // for now, just assume target=i386-pc and assume the disk label is mbr

    // open the device for read+writing
    data->device = checkfopen(prog, data->devicePath, "r+b");

    // check device for a valid MBR label
    fseek(data->device, MBR_BOOTSIG_LOCATION, SEEK_SET);
    uint8_t sigBuf[2];
    size_t nElements = fread(sigBuf, sizeof(sigBuf), 1, data->device);
    if (ferror(data->device)) {
        eprintf(prog, "%s: error occurred when reading MBR\n", data->devicePath);
        quit(prog, EXIT_DEVICE_FAILED);
    }
    if (sigBuf[0] != 0x55 || sigBuf[1] != 0xAA) {
        eprintf(prog, "'%s': invalid MBR\n", data->devicePath);
        quit(prog, EXIT_DEVICE_FAILED);
    }

    // load the bootstrap binary
    uint8_t bootstrap[MBR_PARTITION1];
    data->in = checkfopen(prog, data->bootstrapBin, "rb");
    nElements = fread(bootstrap, sizeof(bootstrap), 1, data->in);
    if (ferror(data->in)) {
        eprintf(prog, "error reading bootstrap '%s'\n", data->bootstrapBin);
        quit(prog, EXIT_BOOTSTRAP_FAILED);
    }
    if (nElements != 1 || !feof(data->in)) {
        eputs(prog, "bootstrap length is invalid");
        quit(prog, EXIT_BOOTSTRAP_FAILED);
    }
    fclose(data->in);

    // we are installing the loader to LBA 1 (this will be different for GPT)
    bootstrap[BOOT_LBA_LOCATION] = 1;

    // install the bootstrap to the boot sector
    fseek(data->device, 0, SEEK_SET);
    nElements = fwrite(bootstrap, sizeof(bootstrap), 1, data->device);
    if (ferror(data->device)) {
        eprintf(prog, "%s: error occurred during bootstrap installation\n", data->devicePath);
        quit(prog, EXIT_DEVICE_FAILED);
    }

    // load the loader binary
    data->in = checkfopen(prog, data->loaderBin, "rb");
    // read the header
    SplHeader header;
    nElements = fread(&header, sizeof(SplHeader), 1, data->in);
    if (ferror(data->in)) {
        eprintf(prog, "error reading loader '%s'\n", data->loaderBin);
        quit(prog, EXIT_LOADER_FAILED);
    }





    return EXIT_SUCCESS;
}