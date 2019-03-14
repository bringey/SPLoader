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
#include "install.h"

#include "target/i386-pc.h"

#define EXIT_DEVICE_FAILED 3
#define EXIT_BOOTSTRAP_FAILED 4
#define EXIT_LOADER_FAILED 5

// shortcut defines
#define MBR SPL_DISK_LABEL_MBR
#define GPT SPL_DISK_LABEL_GPT
#define UNK SPL_DISK_LABEL_UNKNOWN
#define NON SPL_DISK_LABEL_NONE

static Target TARGET_TABLE[] = {
    {
        .name = "i386-pc",
        .endian = SPL_ENDIAN_LITTLE,
        .arch = SPL_ARCH_X86,
        .supportedLabels = MBR | GPT,
        .setDefaults = target_i386pc_setDefaults,
        .installBoot = target_i386pc_installBoot,
        .installLoader = target_i386pc_installLoader
    }
};

#define TARGET_DEFAULT "i386-pc"

#define TARGET_TABLE_LEN (sizeof(TARGET_TABLE) / sizeof(Target))

// static functions

static Target* getTargetByName(char *name);
static SplLabelKind detectLabel(Prog *prog);
static bool detectMbr(Prog *prog);
static bool detectGpt(Prog *prog);




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
            .target = NULL,
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
        {"target", required_argument, NULL, 't'},
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
        c = getopt_long(argc, argv, "b:l:t:v:", longOpts, &longOptIndex);
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
            case 't':
                data->target = getTargetByName(optarg);
                if (data->target == NULL) {
                    eprintf(prog, "unknown target '%s'\n", optarg);
                    return EXIT_BAD_OPTIONS;
                }
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

    // set defaults if needed

    if (data->target == NULL) {
        data->target = getTargetByName(TARGET_DEFAULT);
        nullcheck(data->target);
    }

    Target *target = data->target;
    nullcheck(target->setDefaults);
    nullcheck(target->installBoot);
    nullcheck(target->installLoader);

    target->setDefaults(prog);

    // INSTALLATION PROCEDURE
    // 1. detect the disk label on the target device
    // 2. open the loader binary and check for
    //  * correct architecture for target
    //  * version
    //  * integrity
    // 3. install the bootstrap (if needed)
    // 4. patch loader header + checksum
    // 5. install the loader

    // begin by opening the target device
    data->device = checkfopen(prog, data->devicePath, "r+b");

    // 1. DISK LABEL DETECTION
    SplLabelKind label = detectLabel(prog);
    if (label == SPL_DISK_LABEL_UNKNOWN) {
        eprintf(prog, "'%s' unrecognized disk label\n", data->devicePath);
        return EXIT_UNSUPPORTED_LABEL;
    }


    // CHECK LOADER BINARY
    data->in = checkfopen(prog, data->loaderBin, "rb");
    SplHeader header;
    size_t nelements = fread(&header, sizeof(SplHeader), 1, data->in);
    if (nelements != 1 || ferror(data->in)) {
        eprintf(prog, "'%s': error reading header\n", data->loaderBin);
        return EXIT_LOADER_READ;
    }

    // check for correct arch/endianess
    if (strncmp((char *)(&header.signature), SPL_HEADER_SIGNATURE_STR, sizeof(header.signature)) != 0) {
        return EXIT_HEADER_SIGNATURE;
    }

    if (header.endian != target->endian || header.arch != target->arch) {
        return EXIT_HEADER_TARGET;
    }

    if (!spl_check(&header)) {
        return EXIT_HEADER_CHECKSUM;
    }

    // load the binary
    data->loaderBinBuf = (uint8_t*)malloc(header.loaderSize);
    if (data->loaderBinBuf == NULL) {
        return EXIT_FAILURE;
    }
    fseek(data->in, 0x200, SEEK_SET);
    nelements = fread(data->loaderBinBuf, header.loaderSize, 1, data->in);
    if (nelements != 1 || ferror(data->in)) {
        eprintf(prog, "'%s': error reading binary\n", data->loaderBin);
        return EXIT_LOADER_READ;
    }

    if (!spl_checkBin(&header, data->loaderBinBuf)) {
        return EXIT_HEADER_BINARY_CHECKSUM;
    }


    // UPDATE HEADER

    header.flags = SPL_HEADER_FLAG_ACTIVE;
    header.label = label;
    header.partition = 0;

    // recalculate the headerCrc
    spl_setChecksum(&header);

    // INSTALL BOOTSTRAP

    // INSTALL LOADER

    return EXIT_SUCCESS;
}

SplLabelKind detectLabel(Prog *prog) {
    
    // detection order
    // 1. None
    // 2. MBR
    // 3. GPT
    // the function will check for each of these labels in this order, whilst
    // overriding a previously detected label (since GPT disks also have a MBR)

    // start with none, if we don't detect anything then there's no label
    SplLabelKind label = SPL_DISK_LABEL_NONE;
    
    // check for mbr
    if (detectMbr(prog)) {
        label = SPL_DISK_LABEL_MBR;
    }

    // check for gpt
    if (detectGpt(prog)) {
        label = SPL_DISK_LABEL_GPT;
    }

    // if the detected label is not supported, we will get 0
    // (SPL_DISK_LABEL_UNKNOWN), otherwise return the label
    return label & prog->data->target->supportedLabels;
}

bool detectGpt(Prog *prog) {
    (void)prog;
    return false;
}

bool detectMbr(Prog *prog) {
    ProgData *data = prog->data;
    // check device for a valid MBR label
    fseek(data->device, MBR_BOOTSIG_LOCATION, SEEK_SET);
    uint8_t sigBuf[2];
    size_t nElements = fread(sigBuf, sizeof(sigBuf), 1, data->device);
    if (nElements != 1 || ferror(data->device)) {
        eprintf(prog, "%s: error occurred when reading MBR\n", data->devicePath);
        quit(prog, EXIT_DEVICE_FAILED);
    }
    return sigBuf[0] == 0x55 && sigBuf[1] == 0xAA;
}

static Target* getTargetByName(char *name) {
    Target *target = TARGET_TABLE;
    for (size_t i = 0; i != TARGET_TABLE_LEN; ++i) {
        if (strcmp(target->name, name) == 0) {
            return target;
        }
        ++target;
    }
    return NULL;
}
