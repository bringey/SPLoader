
#ifndef _MEM_H
#define _MEM_H


#ifndef __ASM__

#include <stddef.h>

int spl_mem_init(void);

void spl_mem_dump(void);

void* spl_mem_malloc(size_t bytes);

void* spl_mem_calloc(size_t bytes);

// mem_free not implemented (not needed)
// int mem_free(void *ptr);


#endif  // __ASM__


#endif