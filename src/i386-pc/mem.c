
#include <err.h>
#include <mem.h>

#include <i386-pc/loader.h>
#include <i386-pc/mem.h>

#include <stddef.h>
#include <stdint.h>

#define BIOS_MAP ((E820Map*)LOADER_MMAP_ADDRESS)


size_t _mem_availableBlocks(void) {
    size_t count = 0;
    E820Entry *entry = (E820Entry*)LOADER_MMAP_TABLE;

    // Scan the E820 map provided from BIOS for entries with a type of 1
    // and are 32-bit addresses
    for (unsigned i = 0; i != BIOS_MAP->length; ++i) {
        if (entry->type == 1 && entry->base_hi == 0) {
            if (entry->base_lo >= 0x100000) {
                ++count;
            }
        }
        ++entry;
    }

    return count;
}


size_t _mem_nextBlock(size_t cont, FreeBlock *block) {
    // the continuation value for the i386-pc driver is an index in the E820
    // map. The next index is returned.

    // starting at index cont, scan the E820 map for an entry that is:
    // 1. available (type == 1)
    // 2. 32-bit base address (base_hi == 0)
    E820Entry *entry = ((E820Entry*)LOADER_MMAP_TABLE) + cont;
    for (unsigned i = cont; i != BIOS_MAP->length; ++i) {
        if (entry->type == 1 && entry->base_hi == 0) {

            uint32_t base = entry->base_lo;
            uint32_t limit = entry->base_lo + entry->length_lo;

            if (base < 0x100000) {
                // don't use this range for now
                ++entry;
                continue;
                
                // this range is in low memory and must be adjusted so that it
                // doesn't conflict with the loader code, video ram, ROM and
                // others.

                // for most systems, this range is 0x0-0x9FC00. So we will use
                // the range 0x20000-0x9FC00, assuming that this range fits in
                // the current one.
                // if (base < 0x20000 && limit > 0x20000) {
                //     base = 0x20000;
                // }
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
