
#ifndef _SPLOADER_MEM_FREEBLOCK_H
#define _SPLOADER_MEM_FREEBLOCK_H

#ifndef __ASM__


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


#endif  // __ASM__


#endif