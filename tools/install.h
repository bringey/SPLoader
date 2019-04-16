/*
** File: tools/install.h
**
** Author: bringey
**
*/

#ifndef _INSTALL_H
#define _INSTALL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Target Target;

struct ProgData {
    FILE *in;
    FILE *device;
    uint8_t *loaderBinBuf;
    char *devicePath;
    char *loaderBin;
    char *bootstrapBin;
    Target *target;
    bool verbose;
};

#include "common.h"


struct Target {

    const char *name;
    uint8_t endian;
    uint16_t arch;
    // bitmap of SplDiskLabel that are supported by this target
    uint64_t supportedLabels;
    // Set default values in the Prog struct if needed
    void (*setDefaults)(Prog*);
    // Function pointer for installing the bootsector
    void (*installBoot)(Prog*);
    void (*installLoader)(Prog*);

};

// exit codes

// general (10-99)
#define EXIT_UNSUPPORTED_LABEL      10

// I/O errors (100-199)
#define EXIT_DEVICE_READ            100
#define EXIT_DEVICE_WRITE           101
#define EXIT_LOADER_READ            102
#define EXIT_BOOTSTRAP_READ         103

// invalid header errors (200-299)
#define EXIT_HEADER_SIGNATURE       200
#define EXIT_HEADER_TARGET          201
#define EXIT_HEADER_CHECKSUM        202
#define EXIT_HEADER_BINARY_CHECKSUM 203


#endif // _INSTALL_H