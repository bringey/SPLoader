/*
** File: tools/src/mkbin.c
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

#define EXIT_BAD_OPTIONS 2
#define DEFAULT_BLOCKSIZE 512


typedef struct Prog_s {
    FILE *in;
    FILE *out;
    uint8_t *buf;
    char *name;
} Prog;

typedef struct ProgOpt_s {
    bool editmode;      // if true, the input file is a spl binary to be edited
    bool verbose;
    uint32_t flags;     // flags bitmap to set in the header
    uint32_t label;     // disk label (-l <mbr|gpt>)
    uint32_t partition; // boot partition index (-p <num>)
    char *outputFile;
    char *inputFile;
    uint32_t setmap;    // bitmap of fields to update the header with
} ProgOpt;

static int parse_args(int argc, char *argv[], ProgOpt *opts);
static void quit(Prog *prog, int code);
static void cleanup(Prog *prog);


#define SETMAP_F    0x1
#define SETMAP_L    0x2
#define SETMAP_P    0x4
#define SETMAP_ALL  0xFFFFFFFF

#define eputs(name, msg) \
    fprintf(stderr, "%s: %s\n", name, msg)

#define eprintf(fmt, name, ...) \
    fprintf(stderr, "%s: " fmt, name, ##__VA_ARGS__)

#define usage() \
    fputs("usage: mkbin [-e] [options] <input> [-o <output>]\n", stderr)


FILE* checkfopen(Prog *p, const char *filename, const char *mode) {
    FILE *f = fopen(filename, mode);
    if (f == NULL) {
        eprintf("unable to open file '%s'\n", p->name, filename);
        quit(p, EXIT_FAILURE);
    }
    return f;
}


int parse_args(int argc, char *argv[], ProgOpt *opts) {

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

    

    // default options
    *opts = (ProgOpt){
        .editmode = false,
        .verbose = false,
        .flags = 0,
        .label = SPL_DISK_LABEL_UNKNOWN,
        .partition = 0,
        .inputFile = NULL,
        .outputFile = NULL,
        .setmap = 0
    };

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
                opts->label = label;
                opts->setmap |= SETMAP_L;
                break;
            case 'e':
                opts->editmode = true;
                break;
            case 'f':
                opts->setmap |= SETMAP_F;
                break;
            // case 'l':
            //     opts->setmap |= SETMAP_L;
            //     break;
            case 'o':
                opts->outputFile = optarg;
                break;
            case 'p':
                opts->setmap |= SETMAP_P;
                long num = strtol(optarg, NULL, 10);
                if (num < 0) {
                    eputs(argv[0], "parition index must be positive");
                    badoptions = true;
                } else {
                    opts->partition = (uint32_t)num;
                }
                break;
            case 'v':
                opts->verbose = true;
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
    // x = opts->editmode
    // y = opts->outputFile == NULL
    // test if x != y
    if (opts->editmode != (opts->outputFile == NULL)) {
        const char *msg;
        if (opts->editmode) {
            msg = "-o option cannot be used in edit mode";
        } else {
            msg = "missing output argument";
        }
        eputs(argv[0], msg);
        badoptions = true;
    }

    if (argc - optind != 1) {
        eputs(argv[0], "missing input argument");
        badoptions = true;
    }

    if (badoptions) {
        usage();
        return EXIT_BAD_OPTIONS;
    } else {
        opts->inputFile = argv[optind];
        return EXIT_SUCCESS;
    }
}

bool edit_header(SplHeader *header, ProgOpt *opts) {
    if (opts->setmap == 0) {
        return false;
    } else {
        if (opts->setmap & SETMAP_F) {
            header->flags = opts->flags;
        }

        if (opts->setmap & SETMAP_L) {
            header->label = opts->label;
        }

        if (opts->setmap & SETMAP_P) {
            header->partition = opts->partition;
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
    if (prog->in != NULL) {
        fclose(prog->in);
        prog->in = NULL;
    }
    if (prog->out != NULL) {
        fclose(prog->out);
        prog->out = NULL;
    }
    if (prog->buf != NULL) {
        free(prog->buf);
        prog->buf = NULL;
    }
}


//
// Close all opened files and exit the program with the given error code
// 
void quit(Prog *prog, int code) {
    cleanup(prog);
    exit(code);
}


int main(int argc, char *argv[]) {

    ProgOpt opts;
    int code = parse_args(argc, argv, &opts);
    if (code) {
        return code;
    }

    Prog p = (Prog){
        .in = NULL,
        .out = NULL,
        .name = argv[0]
    };

    SplHeader header;

    if (opts.editmode) {
        // load the header from the input file to edit
        p.in = checkfopen(&p, opts.inputFile, "rb+");
        p.out = p.in;
        size_t bytesRead = fread(&header, sizeof(SplHeader), 1, p.in);
        if (bytesRead != 1) {
            eprintf("error occurred when reading '%s'\n", argv[0], opts.inputFile);
            quit(&p, EXIT_FAILURE);
        }
    } else {
        // read the input file and write it to the output
        p.out = checkfopen(&p, opts.outputFile, "wb");
        p.in = checkfopen(&p, opts.inputFile, "rb");
        size_t blocksize = DEFAULT_BLOCKSIZE;
        uint8_t *buf = (uint8_t*)malloc(blocksize);
        p.buf = buf;
        if (buf == NULL) {
            eputs(argv[0], "unable to allocate memory");
            quit(&p, EXIT_FAILURE);
        }

        fseek(p.out, blocksize, SEEK_SET);
        size_t loaderSize = 0;
        uint32_t loaderCrc = SPL_CRC32_INIT;
        size_t bytesRead;
        while ((bytesRead = fread(buf, sizeof(uint8_t), blocksize, p.in)) != 0) {
            if (bytesRead != blocksize) {
                // we did not read a full block, pad the rest of the buffer
                // with 0
                memset(buf + bytesRead, 0, blocksize - bytesRead);
            }

            if (fwrite(buf, sizeof(uint8_t), blocksize, p.out) != blocksize) {
                eprintf("error occurred when writing to '%s'\n", argv[0], opts.outputFile);
                quit(&p, EXIT_FAILURE);
            }
            loaderSize += bytesRead;
            for (size_t i = 0; i != bytesRead; ++i) {
                loaderCrc = spl_crc32_acc(buf[i], loaderCrc);
            }
        }

        // initialize the header
        memset(&header, 0, sizeof(SplHeader));
        memcpy(header.signature, SPL_HEADER_SIGNATURE_STR, sizeof(header.signature));
        //#if TARGET_ISA == i386
        header.arch = SPL_ARCH_X86;
        header.endian = SPL_ENDIAN_LITTLE;
        //#endif
        header.loaderSize = loaderSize;
        header.loaderCrc = spl_reverse32(~loaderCrc);
        opts.setmap = SETMAP_ALL;
    }

    bool headerChanged = edit_header(&header, &opts);

    if (headerChanged) {
        // seek to the start (to write the header)
        fseek(p.out, 0, SEEK_SET);
        // write the header
        size_t bytesWritten = fwrite(&header, sizeof(SplHeader), 1, p.out);
        if (bytesWritten != 1) {
            eputs(argv[0], "error occurred when writing header");
            quit(&p, EXIT_FAILURE);
        }
    }




    cleanup(&p);
    return EXIT_SUCCESS;
}
