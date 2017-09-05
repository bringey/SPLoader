
#ifndef _SPLOADER_MEM_FREEMAP_H
#define _SPLOADER_MEM_FREEMAP_H

#ifndef __ASM__

#include <SPLoader/mem/FreeBlock.h>

#include <stddef.h>

typedef struct FreeMap_s {

    FreeBlock *freelist;
    FreeBlock *nextblock;
    size_t blockCount;

} FreeMap;

#endif  // __ASM__


#endif