/*
** File: libsploader/label/gpt.c
**
** Author: bringey
**
** Contains the read function for GPT disk labels
*/

#include <sploader.h>

#ifdef SPL_OPT_GPT

int spl_label_gpt_read(SplLabel *label) {
    (void)label;
    // TODO [v0.2.0] Move + refactor ldr_disk_gpt_read here
    return SPL_E_FAILURE;
}

#else

typedef int dummy;

#endif
