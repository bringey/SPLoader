/*
** File: src/disk/gpt.c
**
** Author: bringey
**
** NOTE: This file is an optional compile. To use: define OPT_DISK_GPT and add
**       this file to the LOADER_OBJ list.
*/

#ifdef OPT_DISK_GPT

#include <sploader.h>

#include <loader/assert.h>
#include <loader/disk.h>
#include <loader/disk/gpt.h>
#include <loader/err.h>
#include <loader/string.h>


int disk_gpt_read(DiskLabel *label) {
    (void)label;
    return E_FAILURE;
}


#else

typedef int dummy;

#endif