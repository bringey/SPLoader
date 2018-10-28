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
    // uint32_t byte;
    uint32_t crc = SPL_CRC32_INIT;
    for (size_t i = 0; i != size; ++i) {
        crc = spl_crc32_acc(data[i], crc);
    }

    return spl_crc32_end(crc);
}

uint32_t spl_crc32_acc(uint8_t data, uint32_t crc) {
    uint32_t byte = spl_reverse32(data);
    for (size_t i = 0; i != 8; ++i) {
        if ((int32_t)(crc ^ byte) < 0) {
            crc = (crc << 1) ^ SPL_CRC32_POLYNOMIAL;
        } else {
            crc <<= 1;
        }
        byte <<= 1;
    }
    return crc;
}

uint32_t spl_reverse32(uint32_t num) {
    num = ((num & 0x55555555) << 1) | ((num >> 1) & 0x55555555);
    num = ((num & 0x33333333) << 2) | ((num >> 2) & 0x33333333);
    num = ((num & 0x0F0F0F0F) << 4) | ((num >> 4) & 0x0F0F0F0F);
    return (num << 24) | ((num & 0xFF00) << 8) | ((num >> 8) & 0xFF00) | (num >> 24);
}
