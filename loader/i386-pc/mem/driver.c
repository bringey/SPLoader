
#include <SPLoader/mem/driver.h>
#include <SPLoader/err.h>

#include <SPLoader/i386-pc/mem/map.h>

#include <stddef.h>
#include <stdint.h>

static E820Map *BIOS_MAP = (E820Map*)MEMMAP_LOCATION;


size_t _mem_availableBlocks(void) {
    size_t count = 0;
    E820Entry *entry = (E820Entry*)MEMMAP_TABLE;

    // Scan the E820 map provided from BIOS for entries with a type of 1
    // and are 32-bit addresses
    for (unsigned i = 0; i != BIOS_MAP->length; ++i) {
        if (entry->type == 1 && entry->base_hi == 0) {
            ++count;
        }
        ++entry;
    }

    return count;
}


// int _mem_bootstrap(FreeMap **mapVar) {
//     if (mapVar == NULL) {
//         return E_ARGNULL;
//     }

//     // count the number of memory ranges marked as available

//     unsigned availableCount = 0;
//     E820Entry *entry = (E820Entry*)MEMMAP_TABLE;

//     for (unsigned i = 0; i != BIOS_MAP->length; ++i) {
//         if (entry->type == 1 && entry->base_hi == 0) {
//             // 32-bit base addresses only
//             ++availableCount;
//         }
//         ++entry;
//     }

//     // make sure we actually have memory
//     if (availableCount == 0) {
//         // this shouldn't occur, if it does the BIOS_MAP probably got
//         // corrupted somehow
//         return E_NOMEM; // no ranges marked as available
//     }

//     // determine the size of the FreeMap and FreeList
//     // these structures will be stored in one of the available memory ranges

//     size_t overhead = sizeof(FreeMap) + (sizeof(FreeBlock) * availableCount);

//     // find an available block of memory to use that is <= overhead in size
//     // and initialize the FreeMap there

//     FreeMap *map = NULL;
//     entry = (E820Entry*)MEMMAP_TABLE;
//     for (unsigned i = 0; i != BIOS_MAP->length; ++i) {
//         // the entry must have the following qualifications:
//         //  * available (type == 1)
//         //  * 32-bit base address (base_hi == 0)
//         //  * base address is not in low memory (<= 1MB)
//         //  * the length is greater than or equal to overhead
//         if (entry->type == 1 && entry->base_hi == 0 && entry->base_lo >= 0x100000) {

//             // this entry isn't big enough if and only if
//             // length_hi == 0 and length_lo < overhead
//             if (!(entry->length_hi == 0 && entry->length_lo < overhead)) {
                
//                 // we have a winner
//                 map = (FreeMap*)entry->base_lo;
//                 // the freelist is located right after the map
//                 map->freelist = (FreeBlock*)(map + 1);
//                 map->nextblock = map->freelist;
//                 map->blockCount = availableCount;
//                 break;
//             }

//         }
//         ++entry;
//     }

//     if (map == NULL) {
//         // we failed to find a block big enough for the map
//         return E_NOMEM;
//     }

//     // now initialize the freelist, iterate all available blocks



//     *mapVar = map;
//     return E_SUCCESS;

// }

size_t _mem_nextBlock(size_t cont, FreeBlock *block) {
    // the continuation value for the i386-pc driver is an index in the E820
    // map. The next index is returned.

    // starting at index cont, scan the E820 map for an entry that is:
    // 1. available (type == 1)
    // 2. 32-bit base address (base_hi == 0)
    E820Entry *entry = ((E820Entry*)MEMMAP_TABLE) + cont;
    for (unsigned i = cont; i != BIOS_MAP->length; ++i) {
        if (entry->type == 1 && entry->base_hi == 0) {

            uint32_t base = entry->base_lo;
            uint32_t limit = entry->base_lo + entry->length_lo;

            if (base < 0x100000) {
                // this range is in low memory and must be adjusted so that it
                // doesn't conflict with the loader code, video ram, ROM and
                // others.

                // for most systems, this range is 0x0-0x9FC00. So we will use
                // the range 0x20000-0x9FC00, assuming that this range fits in
                // the current one.
                if (base < 0x20000 && limit > 0x20000) {
                    base = 0x20000;
                }
            }
            block->base = (void*)base;
            block->limit = (void*)limit;
            block->next = (void*)base;
            return i + 1;
        }
        ++entry;
    }

    return MEM_CONT_END; // did not find a block, end of continuation

}

//
// Creates a MemMap from the E820Map created in stage0b
//
// int _mem_getMap(MemMap *map, MemMapEntry *buffer) {

//     if (map == NULL || buffer == NULL) {
//         return E_ARGNULL;
//     }

//     uint32_t entries = BIOS_MAP->length;

//     // temporary buffer for the usable and reserved tables
//     // located right after the E820 memory map
//     MemMapEntry *buffer = (MemMapEntry*)(MEMMAP_TABLE + (sizeof(E820Entry) * entries));

//     map->usableTable = buffer;
//     uint32_t count = 0;

//     // scan the BIOS map for entries of type 1 (available for system use)
//     // put these entries first in the buffer

//     E820Entry entry = biosMap->table;
//     for (unsigned i = 0; i != entries; ++i) {
//         if (entry->type == 1) {
//             buffer->base = entry->base;
//             buffer->length = entry->length;
//             buffer->type = MEM_TYPE_AVAILABLE;
//             ++buffer;
//             ++count;
//         }
//         ++entry;
//     }

//     map->usableCount = count;

//     // scan the BIOS map for entries NOT of type 1 (system reserved)
//     // put these entries second in the buffer

//     count = 0;
//     map->reservedTable = buffer;
//     entry = biosMap->table;

//     for (unsigned i = 0; i != entries; ++i) {
//         if (entry->type != 1) {
//             buffer->base = entry->base;
//             buffer->length = entry->length;
//             buffer->type = entry->type;
//             ++buffer;
//             ++count;
//         }
//         ++entry;
//     }

//     map->reservedCount = count;

//     return E_SUCCESS;
// }


// uint32_t _mem_getMapSize(void) {
//     return BIOS_MAP->length;
// }