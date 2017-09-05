
#ifndef _SPLOADER_MEM_H
#define _SPLOADER_MEM_H


#ifndef __ASM__

#include <stddef.h>

int mem_init(void);

void mem_dump(void);

void* mem_malloc(size_t bytes);

void* mem_calloc(size_t bytes);

// mem_free not implemented (not needed)
// int mem_free(void *ptr);


#endif  // __ASM__


#endif