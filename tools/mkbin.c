/*
** File: tools/mkbin.c
**
** Author: bringey
**
** Adds the SplHeader to a flat loader binary that can be installed to an
** image or device
*/

//#define _GNU_SOURCE
//#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sploader.h>
#include <version.h>

#define USAGE_STR "mkbin [-e] [options] <input> [-o <output>]"
#include "common.h"

#define DEFAULT_BLOCKSIZE 512


struct ProgData {
    FILE *in;
    FILE *out;
    uint8_t *buf;
    // command-line options
    bool editmode;      // if true, the input file is a spl binary to be edited
    bool verbose;
    uint32_t flags;     // flags bitmap to set in the header
    uint32_t label;     // disk label (-l <mbr|gpt>)
    uint32_t partition; // boot partition index (-p <num>)
    char *outputFile;
    char *inputFile;
    uint32_t setmap;    // bitmap of fields to update the header with
};

#define SETMAP_F    0x1
#define SETMAP_L    0x2
#define SETMAP_P    0x4
#define SETMAP_ALL  0xFFFFFFFF

static bool edit_header(Prog *prog, SplHeader *header);

int init(Prog *prog) {
    nullcheck(prog);

    ProgData *data = (ProgData*)malloc(sizeof(ProgData));
    if (data == NULL) {
        return EXIT_FAILURE;
    } else {
        *data = (ProgData){
            .in = NULL,
            .out = NULL,
            .buf = NULL,
            .editmode = false,
            .verbose = false,
            .flags = 0,
            .label = SPL_DISK_LABEL_UNKNOWN,
            .partition = 0,
            .inputFile = NULL,
            .outputFile = NULL,
            .setmap = 0
        };
        prog->data = data;
        return EXIT_SUCCESS;
    }
}


int parse_args(Prog *prog, int argc, char *argv[]) {

    static int label = SPL_DISK_LABEL_UNKNOWN;

    static struct option longOpts[] = {
        {"edit", no_argument, NULL, 'e'},
        {"flags", required_argument, NULL, 'f'},
        //{"label", required_argument, NULL, 'l'},
        {"output", required_argument, NULL, 'o'},
        {"partition", required_argument, NULL, 'p'},
        {"verbose", no_argument, NULL, 'v'},
        // label flags
        {"none", no_argument, &label, SPL_DISK_LABEL_NONE},
        {"mbr", no_argument, &label, SPL_DISK_LABEL_MBR},
        {"gpt", no_argument, &label, SPL_DISK_LABEL_GPT},
        {0, 0, 0, 0}
    };

    nullcheck(prog);
    ProgData *data = prog->data;
    nullcheck(data);

    bool badoptions = false;

    int c;
    int longOptIndex = 0;
    extern int optind, optopt;
    extern char *optarg;
    for (;;) {
        c = getopt_long(argc, argv, "ef:o:p:v:", longOpts, &longOptIndex);
        if (c == EOF) {
            break;
        }

        switch (c) {
            case 0:
                data->label = label;
                data->setmap |= SETMAP_L;
                break;
            case 'e':
                data->editmode = true;
                break;
            case 'f':
                data->setmap |= SETMAP_F;
                break;
            // case 'l':
            //     data->setmap |= SETMAP_L;
            //     break;
            case 'o':
                data->outputFile = optarg;
                break;
            case 'p':
                data->setmap |= SETMAP_P;
                long num = strtol(optarg, NULL, 10);
                if (num < 0) {
                    eputs(prog, "parition index must be positive");
                    badoptions = true;
                } else {
                    data->partition = (uint32_t)num;
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


    // two possible invocations:
    // mkbin -e loader.spl              # edit mode
    // mkbin loader.bin -o loader.spl   # output mode
    //
    // x = data->editmode
    // y = data->outputFile == NULL
    // test if x != y
    if (data->editmode != (data->outputFile == NULL)) {
        const char *msg;
        if (data->editmode) {
            msg = "-o option cannot be used in edit mode";
        } else {
            msg = "missing output argument";
        }
        eputs(prog, msg);
        badoptions = true;
    }

    if (argc - optind != 1) {
        eputs(prog, "missing input argument");
        badoptions = true;
    }

    if (badoptions) {
        usage();
        return EXIT_BAD_OPTIONS;
    } else {
        data->inputFile = argv[optind];
        return EXIT_SUCCESS;
    }
}

int run(Prog *prog) {
    nullcheck(prog);

    ProgData *data = prog->data;
    nullcheck(data);
    SplHeader header;

    if (data->editmode) {
        // load the header from the input file to edit
        data->in = checkfopen(prog, data->inputFile, "rb+");
        data->out = data->in;
        size_t bytesRead = fread(&header, sizeof(SplHeader), 1, data->in);
        if (bytesRead != 1) {
            eprintf(prog, "error occurred when reading '%s'\n", data->inputFile);
            quit(prog, EXIT_FAILURE);
        }
    } else {
        // read the input file and write it to the output
        data->in = checkfopen(prog, data->inputFile, "rb");
        data->out = checkfopen(prog, data->outputFile, "wb");
        size_t blocksize = DEFAULT_BLOCKSIZE;
        uint8_t *buf = (uint8_t*)malloc(blocksize);
        data->buf = buf;
        if (buf == NULL) {
            eputs(prog, "unable to allocate memory");
            quit(prog, EXIT_FAILURE);
        }

        fseek(data->out, blocksize, SEEK_SET);
        size_t loaderSize = 0;
        uint32_t loaderCrc = SPL_CRC32_INIT;
        size_t bytesRead;
        while ((bytesRead = fread(buf, sizeof(uint8_t), blocksize, data->in)) != 0) {
            if (bytesRead != blocksize) {
                // we did not read a full block, pad the rest of the buffer
                // with 0
                memset(buf + bytesRead, 0, blocksize - bytesRead);
            }

            if (fwrite(buf, sizeof(uint8_t), blocksize, data->out) != blocksize) {
                eprintf(prog, "error occurred when writing to '%s'\n", data->outputFile);
                quit(prog, EXIT_FAILURE);
            }
            loaderSize += bytesRead;
            for (size_t i = 0; i != bytesRead; ++i) {
                loaderCrc = spl_crc32_acc(buf[i], loaderCrc);
            }
        }

        // initialize the header
        memset(&header, 0, sizeof(SplHeader));
        memcpy(header.signature, SPL_HEADER_SIGNATURE_STR, sizeof(header.signature));
        const char *version = VERSION_STR;
        size_t vlen = strlen(version);
        if (vlen > sizeof(header.version)) {
            vlen = sizeof(header.version);
        }
        memcpy(header.version, version, vlen);
        //#if TARGET_ISA == i386
        header.arch = SPL_ARCH_X86;
        header.endian = SPL_ENDIAN_LITTLE;
        //#endif
        header.loaderSize = loaderSize;
        header.loaderCrc = spl_reverse32(~loaderCrc);
        data->setmap = SETMAP_ALL;
    }

    bool headerChanged = edit_header(prog, &header);

    if (headerChanged) {
        // seek to the start (to write the header)
        fseek(data->out, 0, SEEK_SET);
        // write the header
        size_t bytesWritten = fwrite(&header, sizeof(SplHeader), 1, data->out);
        if (bytesWritten != 1) {
            eputs(prog, "error occurred when writing header");
            quit(prog, EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}

bool edit_header(Prog *prog, SplHeader *header) {
    nullcheck(prog);
    nullcheck(header);

    ProgData *data = prog->data;
    nullcheck(data);

    if (data->setmap == 0) {
        return false;
    } else {
        if (data->setmap & SETMAP_F) {
            header->flags = data->flags;
        }

        if (data->setmap & SETMAP_L) {
            header->label = data->label;
        }

        if (data->setmap & SETMAP_P) {
            header->partition = data->partition;
        }

        SplHeader copy;
        memcpy(&copy, header, sizeof(SplHeader));
        copy.headerCrc = 0;
        copy.loaderCrc = 0;
        uint32_t crc = spl_crc32((uint8_t*)&copy, sizeof(SplHeader));
        header->headerCrc = crc;
        return true;
    }

}


void cleanup(Prog *prog) {
    nullcheck(prog);

    ProgData *data = prog->data;
    if (data != NULL) {
        if (data->in != NULL) {
            fclose(data->in);
            data->in = NULL;
        }
        if (data->out != NULL) {
            fclose(data->out);
            data->out = NULL;
        }
        if (data->buf != NULL) {
            free(data->buf);
            data->buf = NULL;
        }
    }
    free(data);
    prog->data = NULL;
}
