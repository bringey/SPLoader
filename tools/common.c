/*
** File: tools/common.c
**
** Author: bringey
**
*/

#include <stdlib.h>

#include "common.h"


FILE* checkfopen(Prog *p, const char *filename, const char *mode) {
    FILE *f = fopen(filename, mode);
    if (f == NULL) {
        eprintf(p, "unable to open file '%s'\n", filename);
        quit(p, EXIT_FAILURE);
    }
    return f;
}


void quit(Prog *prog, int exitcode) {
    cleanup(prog);
    exit(exitcode);
}

int main(int argc, char *argv[]) {
    Prog prog;
    int exitcode;

    prog.name = argv[0];
    exitcode = init(&prog);
    if (exitcode)
        return exitcode;

    exitcode = parse_args(&prog, argc, argv);
    if (exitcode == 0) {
        exitcode = run(&prog);
    }

    cleanup(&prog);
    return exitcode;
}
