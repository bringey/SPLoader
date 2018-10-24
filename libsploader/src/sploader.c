/*
** File: libsploader/src/sploader.c
**
** Author: bringey
**
*/

#include <sploader.h>

static uint32_t reverse(uint32_t num);


int spl_check(SplHeader *header) {
    (void)header;
    return SPL_HEADER_SUCCESS;
}

uint32_t spl_crc32(uint8_t data[], size_t size) {
    uint32_t byte;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i != size; ++i) {
        byte = reverse(data[i]);
        for (size_t j = 0; j != 8; ++j) {
            if ((int)(crc ^ byte) < 0) {
                crc = (crc << 1) ^ 0x04C11DB7;
            } else {
                crc <<= 1;
            }
            byte <<= 1;
        }
    }

    return reverse(~crc);
}

uint32_t reverse(uint32_t num) {
    num = ((num & 0x55555555) << 1) | ((num >> 1) & 0x55555555);
    num = ((num & 0x33333333) << 2) | ((num >> 2) & 0x33333333);
    num = ((num & 0x0F0F0F0F) << 4) | ((num >> 4) & 0x0F0F0F0F);
    return (num << 24) | ((num & 0xFF00) << 8) | ((num >> 8) & 0xFF00) | (num >> 24);
}