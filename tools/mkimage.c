/*
** File: tools/mkimage.c
**
** mkimage <image files> -o <output_file>
**
*/


#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <loader/disk/mbr.h>
#include <loader/i386-pc/stage0a/bootstrap.h>

#include "common.h"

#define USAGE_STR "mkimage <loader binary> -o <output_file>"

#define SECTOR_SIZE 512

#define i32le(num)

struct ProgData {
    FILE *in;
    FILE *out;
    uint8_t *buf;
    // command line args
    char *loaderBin;
    char *bootstrapBin;
    char *outputFile;
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
            .out = NULL,
            .buf = NULL,
            .loaderBin = NULL,
            .bootstrapBin = NULL,
            .outputFile = NULL,
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
        if (data->out != NULL) {
            fclose(data->out);
            data->out = NULL;
        }
        if (data->buf != NULL) {
            free(data->buf);
            data->buf = NULL;
        }

        free(data);
        prog->data = NULL;
    }
}

int parse_args(Prog *prog, int argc, char *argv[]) {

    static struct option longOpts[] = {
        {"bootstrap", required_argument, NULL, 'b'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };


    ProgData *data = prog->data;

    bool badoptions = false;

    int c;
    int longOptIndex = 0;
    extern int optind, optopt;
    extern char *optarg;

    opterr = 1;
    for (;;) {
        c = getopt_long(argc, argv, "b:o:v", longOpts, &longOptIndex);
        if (c == EOF) {
            break;
        }

        switch (c) {
            case 0:
                break;
            case 'b':
                data->bootstrapBin = optarg;
                break;
            case 'o':
                data->outputFile = optarg;
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
        eputs(prog, "no bootstrap binary specified");
        badoptions = true;
    }

    if (data->outputFile == NULL) {
        eputs(prog, "no output file specified");
        badoptions = true;
    }

    if (argc - optind != 1) {
        eputs(prog, "missing loader binary");
        badoptions = true;
    }

    if (badoptions) {
        usage();
        return EXIT_BAD_OPTIONS;
    } else {
        data->loaderBin = argv[optind];
        return EXIT_SUCCESS;
    }
}

int run(Prog *prog) {

    nullcheck(prog);
    ProgData *data = prog->data;

    size_t blocksize = SECTOR_SIZE;
    data->buf = (uint8_t*)calloc(blocksize, sizeof(uint8_t));
    if (data->buf == NULL) {
        eputs(prog, "out of memory");
        quit(prog, EXIT_FAILURE);
    }
    data->in = checkfopen(prog, data->bootstrapBin, "rb");
    size_t nElements = fread(data->buf, sizeof(uint8_t), blocksize, data->in);
    if (ferror(data->in)) {
        eprintf(prog, "error reading bootstrap '%s'\n", data->bootstrapBin);
        quit(prog, EXIT_FAILURE);
    }
    if (nElements > 446) {
        eprintf(prog, "'%s' is too large\n", data->bootstrapBin);
        quit(prog, EXIT_FAILURE);
    }
    // set BOOT_LBA in the footer to LBA 1
    // little-endian so we can just write the first byte
    data->buf[BOOT_LBA_LOCATION] = 1;

    // set the mbr signature
    data->buf[MBR_BOOTSIG_LOCATION + 0] = 0x55; // set by byte so this works on big endian systems
    data->buf[MBR_BOOTSIG_LOCATION + 1] = 0xAA;

    // write the bootstrap to the image
    data->out = checkfopen(prog, data->outputFile, "wb");
    nElements = fwrite(data->buf, sizeof(uint8_t), blocksize, data->out);
    if (nElements != blocksize) {
        eprintf(prog, "failed to write bootstrap to '%s'\n", data->outputFile);
        quit(prog, EXIT_FAILURE);
    }

    // copy the loader binary to the output
    
    fclose(data->in); // we're done with bootstrap.bin
    data->in = checkfopen(prog, data->loaderBin, "rb");
    while (!feof(data->in)) {
        nElements = fread(data->buf, sizeof(uint8_t), blocksize, data->in);
        if (ferror(data->in)) {
            eprintf(prog, "error reading '%s'\n", data->loaderBin);
            quit(prog, EXIT_FAILURE);
        }
        if (nElements != blocksize) {
            memset(data->buf + nElements, 0, blocksize - nElements);
        }

        if (fwrite(data->buf, sizeof(uint8_t), blocksize, data->out) != blocksize) {
            eprintf(prog, "failed to write loader to '%s'\n", data->outputFile);
            quit(prog, EXIT_FAILURE);
        }
    }



    return EXIT_SUCCESS;
}


// uint16_t copyfile(Prog *prog, char *filename) {
//     prog->in = fopen(filename, "rb");
//     if (prog->in == NULL) {
//         fprintf(stderr, "error opening file '%s'\n", filename);
//         quit(prog, EXIT_FAILURE);
//     }

//     #define BUF_SIZE 512
//     char buf[BUF_SIZE];
//     int bytesRead;
//     int byteTotal = 0;

//     while ((bytesRead = fread(buf, sizeof(char), BUF_SIZE, prog->in)) != 0) {
//         if (bytesRead != BUF_SIZE) {
//             for (int i = bytesRead; i != BUF_SIZE; ++i) {
//                 buf[i] = '\0';
//             }
//         }

//         if (fwrite(buf, sizeof(char), BUF_SIZE, prog->out) != BUF_SIZE) {
//             fputs("error: write failed\n", stderr);
//             quit(prog, EXIT_FAILURE);
//         }

//         byteTotal += BUF_SIZE;
//     }

//     fclose(prog->in);
//     prog->in = NULL;

//     printf("%s: %d sectors\n", filename, byteTotal / BUF_SIZE);
//     return (uint16_t)byteTotal;
// }


// int main(int argc, char* argv[]) {

//     Prog prog = {
//         .out = NULL,
//         .in = NULL
//     };

//     char *outfileName = NULL;
//     char *bootstrap = NULL;
//     //char *loader16 = NULL;
//     char *loader = NULL;

//     int c;
//     extern int optind, optopt;
//     extern char *optarg;

//     while ((c = getopt(argc, argv, ":b:o:")) != EOF) {

//         switch (c) {
//             case ':':
//                 fprintf(stderr, "missing operand after -%c\n", optopt);
//                 // fall through
//             case '?':
//                 usage();
//                 break;
//             case 'b':
//                 bootstrap = optarg;
//                 break;
//             case 'o':
//                 outfileName = optarg;
//                 break;
//             default:
//                 usage();
//                 break;

//         }

//     }

//     if (bootstrap == NULL) {
//         fputs("no bootstrap binary specified\n", stderr);
//         usage();
//     }

//     if (outfileName == NULL) {
//         fputs("no output file specified\n", stderr);
//         usage();
//     }

//     if (argc - optind != 1) {
//         fputs("invalid amount of loader binaries\n", stderr);
//         usage();
//     }

//     //loader16 = argv[optind];
//     loader = argv[optind];

//     prog.out = fopen(outfileName, "wb");
//     if (prog.out == NULL) {
//         fprintf(stderr, "error opening file '%s'\n", outfileName);
//         quit(&prog, EXIT_FAILURE);
//     }

//     copyfile(&prog, bootstrap);

//     BootHeader header = {0};
//     strcpy((char*)&header, "SPLOADER");
//     fwrite(&header, sizeof(BootHeader), 1, prog.out);

//     //uint16_t loader16_size = copyfile(&prog, loader16);
//     uint16_t loader_size = copyfile(&prog, loader);

//     // write LBA of boot partition (always 1)
//     uint32_t boot_lba = 1;
//     fseek(prog.out, BOOT_LBA_LOCATION, SEEK_SET);
//     fwrite(&boot_lba, sizeof(uint32_t), 1, prog.out);

//     // write loader.bin size in header
//     fseek(prog.out, 0x200 + BOOT_HEADER_LOADER_SIZE, SEEK_SET);
//     fwrite(&loader_size, sizeof(uint16_t), 1, prog.out);

//     // uint32_t loader_lba = 1;
    
//     // fseek(prog.out, LOADER_LBA_LOCATION, SEEK_SET);
//     // fwrite(&loader_lba, sizeof(uint32_t), 1, prog.out);
    
//     // fseek(prog.out, LOADER_SIZE_LOCATION, SEEK_SET);
//     // fwrite(&loader_size, sizeof(uint16_t), 1, prog.out);

//     // fseek(prog.out, LOADER16_SIZE_LOCATION, SEEK_SET);
//     // fwrite(&loader16_size, sizeof(uint16_t), 1, prog.out);

//     uint16_t mbrsig = MBR_BOOTSIG;
//     fseek(prog.out, MBR_BOOTSIG_LOCATION, SEEK_SET);
//     fwrite(&mbrsig, sizeof(uint16_t), 1, prog.out);

//     quit(&prog, EXIT_SUCCESS);

//     return 0;
// }