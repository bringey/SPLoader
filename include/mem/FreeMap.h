
#ifndef _MEM_FREEMAP_H
#define _MEM_FREEMAP_H

#ifndef __ASM__

#include <mem/FreeBlock.h>

#include <stddef.h>

typedef struct FreeMap_s {

    FreeBlock *freelist;
    FreeBlock *nextblock;
    size_t blockCount;

} FreeMap;

#endif  // __ASM__


#endif