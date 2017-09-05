
#ifndef _SPLOADER_MEM_DRIVER_H
#define _SPLOADER_MEM_DRIVER_H

#define MEM_CONT_START 0
#define MEM_CONT_END -1


#ifndef __ASM__

#include <SPLoader/mem/FreeMap.h>

#include <stddef.h>

int _mem_bootstrap(FreeMap **map);

size_t _mem_availableBlocks(void);

size_t _mem_nextBlock(size_t cont, FreeBlock *block);

#endif  // __ASM__


#endif