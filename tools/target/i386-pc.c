/*
** File: tools/target/i386-pc.c
**
** Author: bringey
**
*/

#include "i386-pc.h"
#include "../install.h"

#include <stdlib.h>

void target_i386pc_setDefaults(Prog* prog) {
    // we'll have default locations for these when distributing, for now
    // just error out.
    if (prog->data->bootstrapBin == NULL) {
        eputs(prog, "missing bootstrap binary");
        quit(prog, EXIT_FAILURE);
    }
    if (prog->data->loaderBin == NULL) {
        eputs(prog, "missing loader binary");
        quit(prog, EXIT_FAILURE);
    }
}

void target_i386pc_installBoot(Prog* prog) {
    (void)prog;
    // todo
}

void target_i386pc_installLoader(Prog *prog) {
    (void)prog;
    // todo
}