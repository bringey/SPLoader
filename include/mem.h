/*
** File: include/mem.h
**
** Author: bringey
*/

#ifndef _MEM_H
#define _MEM_H

#define MEM_CONT_START 0
#define MEM_CONT_END -1

#ifndef __ASM__

#include <stddef.h>

int spl_mem_init(void);

void spl_mem_dump(void);

void* spl_mem_malloc(size_t bytes);

void* spl_mem_calloc(size_t bytes);

// mem_free not implemented (not needed)
// int mem_free(void *ptr);

// ============================================================================
// Driver functions


typedef struct FreeBlock_s FreeBlock;

//
// Typedef for a FreeBlock structure. A FreeBlock consists of a memory range
// that is available for use. The memory range is specified by the base and
// limit pointers. The next pointer points to the next available memory
// location to be allocated. If the next and limit pointers are equivalent,
// then this block of memory is used up and no allocations are possible for this
// block. The base pointer should NEVER be larger than the limit pointer.
//
struct FreeBlock_s {

    void *next;         // next available memory location to allocate
    void *base;         // starting address of memory range
    void *limit;        // ending address of memory range

};


typedef struct FreeMap_s {

    FreeBlock *freelist;
    FreeBlock *nextblock;
    size_t blockCount;

} FreeMap;

#undef MEM_CONT_END
#define MEM_CONT_END ((size_t)-1)

int _spl_mem_bootstrap(FreeMap **map);

size_t _spl_mem_availableBlocks(void);

size_t _spl_mem_nextBlock(size_t cont, FreeBlock *block);


#endif  // __ASM__


#endif