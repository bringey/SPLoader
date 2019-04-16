/*
** File: tools/common.h
**
** Author: bringey
**
** Private header containing functions and macros shared by each tool.
*/



#ifndef _COMMON_PH
#define _COMMON_PH

#include <stdio.h>
#include <assert.h>

#define EXIT_BAD_OPTIONS 2

#define eputs(prog, msg) \
    fprintf(stderr, "%s: %s\n", prog->name, msg)

#define eprintf(prog, fmt, ...) \
    fprintf(stderr, "%s: " fmt, prog->name, ##__VA_ARGS__)

#define usage() \
    fputs("usage: " USAGE_STR "\n", stderr)

// assert that an expression != NULL
#define nullcheck(expr) assert((expr) != NULL)

typedef struct ProgData ProgData;

typedef struct Prog {
    const char *name; // argv[0]
    ProgData *data;
} Prog;

//
// fopen wrapper with error-handling
//
FILE* checkfopen(Prog *prog, const char *filename, const char *mode);

void quit(Prog *prog, int exitcode);

//
// These functions are to be implemented for each tool executable
//

int init(Prog *prg);

int parse_args(Prog *prg, int argc, char *argv[]);

int run(Prog *prog);

void cleanup(Prog *prog);

#endif
