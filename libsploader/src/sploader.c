/*
** File: libsploader/src/sploader.c
**
** Author: bringey
**
*/

#include <sploader.h>



int spl_check(SplHeader *header) {
    (void)header;
    return SPL_HEADER_SUCCESS;
}

uint32_t spl_crc32(uint8_t data[], size_t size) {
    (void)data; (void)size;
    return 0;
}