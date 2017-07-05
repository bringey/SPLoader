/*
** File: loader/i386-pc/mem/map.c
**
** Author: Brennan Ringey
**
*/

#include <SPLoader/i386-pc/mem/map.h>
#include <SPLoader/console/out.h>

static MemMap *MEMMAP = (MemMap*)MEMMAP_LOCATION;
static MemMapEntry *TABLE = (MemMapEntry*)MEMMAP_TABLE;

static char* PREFIXES[] = {
    "B",
    "KiB",
    "MiB",
    "GiB"
};

static uint32_t __prettylength(uint32_t lenHi, uint32_t lenLo, char **prefix);

void memmap_dump(bool showAvailable) {

    uint32_t len = MEMMAP->length;

    con_printf("[MemMap] length           = %d\n", len);
    con_printf("[MemMap] length_per_entry = %d\n", MEMMAP->length_per_entry);

    //con_puts("[   Base Addr   ] [    Length     ] [Type]  [              Range              ]\n");
    con_puts("     Base Addr     |      Length                 | Type\n");
    con_puts("-------------------+-----------------------------+------------------------------");

    MemMapEntry *e = TABLE;
    for (unsigned i = 0; i < len; ++i, ++e) {
        if (showAvailable && e->type != 1) {
            continue; // entry is reserved, do not dump it
        }
        memmap_dumpEntry(e);
    }

}

void memmap_dumpEntry(MemMapEntry *entry) {

    con_printf(" %08x %08x |", entry->base_hi, entry->base_lo);
    con_printf(" %08x %08x ", entry->length_hi, entry->length_lo);

    char *prefix;
    uint32_t len = __prettylength(entry->length_hi, entry->length_lo, &prefix);
    con_printf(" %4d %3s |", len, prefix);
    
    con_printf(" %x\n", entry->type);

    //con_printf(" %16d\n", entry->length_lo / 1024);
    

    //con_printf(" %08x%08x-", (rangeStart >> 16) >> 16, rangeStart & 0xFFFFFFFF);
    //con_printf("%08x%08x\n", (rangeEnd >> 16) >> 16, rangeEnd & 0xFFFFFFFF);


}

//
// the following functions take a 64-bit length (in bytes), as two 32-bit ints,
// and reduce it to a value between 0 and 1023 with a prefix (B, KiB, MiB, etc)
// this is done by dividing the length by 1024 recursively until the length is
// between 0 and 1023. The number of divisions determines the prefix, which
// this number is used as an index to an array of prefixes.
//
// Integer division is used, so any remainders during division will be lost
//
// Examples:
//  __prettylength(0, 1024, &prefix) => 1, prefix = "KiB"  (Actual: 1 KiB)
//  __prettylength(1, 0, &prefix)    => 4, prefix = "GiB"  (Actual: 4 GiB)
//  __prettylength(0, 0xBD5A1000, &prefix) => 2, prefix = "GiB" (Actual: 2.96 GiB)
//

uint32_t __prettylengthAux(uint32_t lenHi, uint32_t lenLo, int *index) {
    if (lenHi == 0 && lenLo < 1024) {
        return lenLo;
    } else {
        // divide by 1024 (in this case shift right by 10)
        // shifting bits from high to low
        lenLo = (lenLo >> 10) | ((lenHi & 0x3FF) << 22);
        lenHi >>= 10;
        ++(*index);
        return __prettylengthAux(lenHi, lenLo, index);
    }
}

static uint32_t __prettylength(uint32_t lenHi, uint32_t lenLo, char **prefix) {
    int index = 0;
    uint32_t len = __prettylengthAux(lenHi, lenLo, &index);
    *prefix = PREFIXES[index];
    return len;
}
