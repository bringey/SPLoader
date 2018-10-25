/*
** File: tools/src/mkimage.c
**
** mkimage <image files> -o <output_file>
**
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <loader/disk/mbr.h>
#include <loader/i386-pc/stage0a/bootstrap.h>
#include <loader/i386-pc/BootHeader.h>


#define SECTOR_SIZE 512

typedef struct {
    FILE *out;
    FILE *in;
} Prog;



void usage(void) {
    fputs("usage: mkimage <loader binary> -o <output_file>\n", stderr);
    exit(EXIT_FAILURE);
}

void quit(Prog *prog, int code) {
    if (prog->out != NULL) {
        fclose(prog->out);
        prog->out = NULL;
    }

    if (prog->in != NULL) {
        fclose(prog->in);
        prog->in = NULL;
    }

    exit(code);
}

uint16_t copyfile(Prog *prog, char *filename) {
    prog->in = fopen(filename, "rb");
    if (prog->in == NULL) {
        fprintf(stderr, "error opening file '%s'\n", filename);
        quit(prog, EXIT_FAILURE);
    }

    #define BUF_SIZE 512
    char buf[BUF_SIZE];
    int bytesRead;
    int byteTotal = 0;

    while ((bytesRead = fread(buf, sizeof(char), BUF_SIZE, prog->in)) != 0) {
        if (bytesRead != BUF_SIZE) {
            for (int i = bytesRead; i != BUF_SIZE; ++i) {
                buf[i] = '\0';
            }
        }

        if (fwrite(buf, sizeof(char), BUF_SIZE, prog->out) != BUF_SIZE) {
            fputs("error: write failed\n", stderr);
            quit(prog, EXIT_FAILURE);
        }

        byteTotal += BUF_SIZE;
    }

    fclose(prog->in);
    prog->in = NULL;

    printf("%s: %d sectors\n", filename, byteTotal / BUF_SIZE);
    return (uint16_t)byteTotal;
}


int main(int argc, char* argv[]) {

    Prog prog = {
        .out = NULL,
        .in = NULL
    };

    char *outfileName = NULL;
    char *bootstrap = NULL;
    //char *loader16 = NULL;
    char *loader = NULL;

    int c;
    extern int optind, optopt;
    extern char *optarg;

    while ((c = getopt(argc, argv, ":b:o:")) != EOF) {

        switch (c) {
            case ':':
                fprintf(stderr, "missing operand after -%c\n", optopt);
                // fall through
            case '?':
                usage();
                break;
            case 'b':
                bootstrap = optarg;
                break;
            case 'o':
                outfileName = optarg;
                break;
            default:
                usage();
                break;

        }

    }

    if (bootstrap == NULL) {
        fputs("no bootstrap binary specified\n", stderr);
        usage();
    }

    if (outfileName == NULL) {
        fputs("no output file specified\n", stderr);
        usage();
    }

    if (argc - optind != 1) {
        fputs("invalid amount of loader binaries\n", stderr);
        usage();
    }

    //loader16 = argv[optind];
    loader = argv[optind];

    prog.out = fopen(outfileName, "wb");
    if (prog.out == NULL) {
        fprintf(stderr, "error opening file '%s'\n", outfileName);
        quit(&prog, EXIT_FAILURE);
    }

    copyfile(&prog, bootstrap);

    BootHeader header = {0};
    strcpy((char*)&header, "SPLOADER");
    fwrite(&header, sizeof(BootHeader), 1, prog.out);

    //uint16_t loader16_size = copyfile(&prog, loader16);
    uint16_t loader_size = copyfile(&prog, loader);

    // write LBA of boot partition (always 1)
    uint32_t boot_lba = 1;
    fseek(prog.out, BOOT_LBA_LOCATION, SEEK_SET);
    fwrite(&boot_lba, sizeof(uint32_t), 1, prog.out);

    // write loader.bin size in header
    fseek(prog.out, 0x200 + BOOT_HEADER_LOADER_SIZE, SEEK_SET);
    fwrite(&loader_size, sizeof(uint16_t), 1, prog.out);

    // uint32_t loader_lba = 1;
    
    // fseek(prog.out, LOADER_LBA_LOCATION, SEEK_SET);
    // fwrite(&loader_lba, sizeof(uint32_t), 1, prog.out);
    
    // fseek(prog.out, LOADER_SIZE_LOCATION, SEEK_SET);
    // fwrite(&loader_size, sizeof(uint16_t), 1, prog.out);

    // fseek(prog.out, LOADER16_SIZE_LOCATION, SEEK_SET);
    // fwrite(&loader16_size, sizeof(uint16_t), 1, prog.out);

    uint16_t mbrsig = MBR_BOOTSIG;
    fseek(prog.out, MBR_BOOTSIG_LOCATION, SEEK_SET);
    fwrite(&mbrsig, sizeof(uint16_t), 1, prog.out);

    quit(&prog, EXIT_SUCCESS);

    return 0;
}