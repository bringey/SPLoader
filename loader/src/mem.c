/*
** File: src/mem.c
**
** Author: bringey
*/

#include <loader/assert.h>
#include <loader/console.h>
#include <loader/err.h>
#include <loader/mem.h>

#include <stdbool.h>
#include <stdint.h>

static void* __allocate(FreeBlock *block, size_t bytes);

static FreeMap *map;

#define TEMP_MAP_LENGTH 100


int ldr_mem_init(void) {

    size_t blockCount = _ldr_mem_availableBlocks();
    if (blockCount == 0) {
        return E_NOMEM;
    }

    size_t overhead = sizeof(FreeMap) + (blockCount * sizeof(FreeBlock));

    // find a block that will fit the freelist (must be <= overhead)
    void *homebase = NULL;
    FreeBlock block;
    size_t cont = MEM_CONT_START;
    while ((cont = _ldr_mem_nextBlock(cont, &block)) != MEM_CONT_END) {
        if (overhead <= (size_t)block.limit - (size_t)block.base) {
            homebase = (void*)block.base;
            break;
        }
    }

    if (homebase == NULL) {
        return E_NOMEM;
    }

    map = (FreeMap*)homebase;
    map->freelist = (void*)((size_t)homebase + sizeof(FreeMap));
    map->nextblock = map->freelist;
    map->blockCount = blockCount;

    // con_printf("freemap: %08x\n", map);
    // con_printf("freelist: %08x\n", map->freelist);

    // populate the freelist

    FreeBlock *freelistIter = map->freelist;
    cont = MEM_CONT_START;
    while ((cont = _ldr_mem_nextBlock(cont, &block)) != MEM_CONT_END) {
        if (block.base == homebase) {
            block.base = (void*)((size_t)block.base + overhead);
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

void ldr_mem_dump(void) {

    ldr_con_printf("[FreeMap] %08x: freelist: %08x nextblock: %08x blocks: %d\n",
                   map, map->freelist, map->nextblock, map->blockCount);
    
    FreeBlock *block = map->freelist;
    for (unsigned i = 0; i != map->blockCount; ++i) {
        ldr_con_printf("[FreeBlock] %08x: (%08x-%08x) next: %08x\n",
                       block, block->base, block->limit, block->next);
        ++block;
    }

}

void* ldr_malloc(size_t bytes) {
    if (bytes == 0) {
        return NULL;
    }

    if (map->nextblock == NULL) {
        return NULL;
    }

    return __allocate(map->nextblock, bytes);

}

int ldr_free(void *ptr) {

    ldr_assert(map->lastalloc == ptr);

    // find the block that contains the pointer
    bool found = false;
    FreeBlock *block = map->freelist;
    for (size_t i = 0; i != map->blockCount; ++i) {
        if (found) {
            // reset all freeblocks past the block that contained the pointer
            block->next = block->base;
        } else {
            if (ptr >= block->base && ptr < block->limit) {
                // found the pointer to free, set the next alloc to this
                // pointer
                found = true;
                block->next = ptr;
            }
        }
        ++block;
    }

    return found ? E_SUCCESS : E_FAILURE;
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

    if (result != NULL) {
        map->lastalloc = result;
    }
    return result;
}