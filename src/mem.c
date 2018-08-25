/*
** File: src/mem.c
**
** Author: bringey
*/

#include <console.h>
#include <err.h>
#include <mem.h>

#include <stdint.h>

static void* __allocate(FreeBlock *block, size_t bytes);

static FreeMap *map;

#define TEMP_MAP_LENGTH 100


int spl_mem_init(void) {

    size_t blockCount = _spl_mem_availableBlocks();
    if (blockCount == 0) {
        return E_NOMEM;
    }

    size_t overhead = sizeof(FreeMap) + (blockCount * sizeof(FreeBlock));

    // find a block that will fit the freelist (must be <= overhead)
    void *homebase = NULL;
    FreeBlock block;
    size_t cont = MEM_CONT_START;
    while ((cont = _spl_mem_nextBlock(cont, &block)) != MEM_CONT_END) {
        if (overhead <= (uint32_t)block.limit - (uint32_t)block.base) {
            homebase = (void*)block.base;
            break;
        }
    }

    if (homebase == NULL) {
        return E_NOMEM;
    }

    map = (FreeMap*)homebase;
    map->freelist = (void*)((uint32_t)homebase + sizeof(FreeMap));
    map->nextblock = map->freelist;
    map->blockCount = blockCount;

    // spl_con_printf("freemap: %08x\n", map);
    // spl_con_printf("freelist: %08x\n", map->freelist);

    // populate the freelist

    FreeBlock *freelistIter = map->freelist;
    cont = MEM_CONT_START;
    while ((cont = _spl_mem_nextBlock(cont, &block)) != MEM_CONT_END) {
        if (block.base == homebase) {
            block.base = (void*)((uint32_t)block.base + overhead);
            block.next = block.base;
        }
        *freelistIter++ = block;
    }


    // sort the freelist, the blocks returned from _mem_nextBlock are not
    // guaranteed to be in the correct order, and some blocks may be overlapping
    // uint32_t lastBase = 0;
    // uint32_t lastLimit = 0;
    // freelistIter = map->freelist;
    // for (unsigned i = 0; i != blockCount; ++i) {
        
    //     ++freelistIter;
    // }


    return E_SUCCESS;
}

void spl_mem_dump(void) {

    spl_con_printf("[FreeMap] %08x: freelist: %08x nextblock: %08x blocks: %d\n",
               map, map->freelist, map->nextblock, map->blockCount);
    
    FreeBlock *block = map->freelist;
    for (unsigned i = 0; i != map->blockCount; ++i) {
        spl_con_printf("[FreeBlock] %08x: (%08x-%08x) next: %08x\n",
                   block, block->base, block->limit, block->next);
        ++block;
    }

}

void* spl_mem_malloc(size_t bytes) {
    if (bytes == 0) {
        return NULL;
    }

    if (map->nextblock == NULL) {
        return NULL;
    }

    return __allocate(map->nextblock, bytes);

}


void* __allocate(FreeBlock *block, size_t bytes) {
    size_t space = (size_t)block->limit - (size_t)block->next;
    void *result = NULL;

    if (bytes > space) {
        // the current block does not have enough space for this allocation
        // try the next one
        ptrdiff_t index = block - map->freelist;
        ++index;
        if ((size_t)index < map->blockCount) {
            result = __allocate(map->freelist + index, bytes);
        }
    } else {
        result = block->next;
        block->next = (void*)((size_t)block->next + bytes);
        if (bytes == space) {
            // this allocation completely fills up the block, modify nextblock
            ptrdiff_t index = block - map->freelist;
            if ((size_t)(++index) < map->blockCount) {
                map->nextblock = map->freelist + index;
            } else {
                map->nextblock = NULL; // out of memory
            }
        }
    }

    return result;
}