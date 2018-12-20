/*
** File: include/loader/mem.h
**
** Author: bringey
*/

#ifndef _LOADER_MEM_H
#define _LOADER_MEM_H

#define MEM_CONT_START 0
#define MEM_CONT_END -1

#ifndef __ASM__

#include <stddef.h>

int ldr_mem_init(void);

void ldr_mem_dump(void);

void* ldr_malloc(size_t bytes);

void* ldr_calloc(size_t bytes);

//
// free implementation, only works for the LAST pointer returned by mem_malloc
// or mem_calloc. Used for temporary buffers.
//
int ldr_free(void *ptr);

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
    void *lastalloc;

} FreeMap;

#undef MEM_CONT_END
#define MEM_CONT_END ((size_t)-1)

int _ldr_mem_bootstrap(FreeMap **map);

size_t _ldr_mem_availableBlocks(void);

size_t _ldr_mem_nextBlock(size_t cont, FreeBlock *block);


#endif  // __ASM__


#endif