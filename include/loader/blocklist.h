/*
** File: include/loader/blocklist.h
**
** Author: bringey
**
** Utility module containing the blocklist data structure. A blocklist is a
** sorted list of blocks (base and limit pairs) that can be used to map out
** free memory or a disk partition layout. Checking for overlapping blocks
*/

#ifndef _LOADER_BLOCKLIST_H
#define _LOADER_BLOCKLIST_H

#define BLOCKLIST_GREATER 1
#define BLOCKLIST_OVERLAPS 0
#define BLOCKLIST_LESSER -1

#ifndef __ASM__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


typedef struct Block32_s {
    uint32_t base;
    uint32_t limit;
    void *aux;
} Block32;

typedef struct Blocklist_s {
    Block32 *blocks;
    size_t len;
    size_t capacity;
} Blocklist;


int blocklist_insert(Blocklist *list, Block32 block, bool mergeOverlap);

int blocklist_cmp(Block32 b1, Block32 b2);


#endif

#endif