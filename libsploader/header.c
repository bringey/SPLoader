/*
** File: libsploader/header.c
**
** Author: bringey
**
** Contains implementations for all spl_hdr_* functions
*/

#include <sploader.h>

bool spl_hdr_check(SplHeader *header) {

    SplHeader copy = *header;
    copy.loaderCrc = 0;
    copy.headerCrc = 0;
    uint32_t crc = spl_crc32(&copy, sizeof(SplHeader));

    return crc == header->headerCrc;
}

bool spl_hdr_checkBin(SplHeader *header, void *binary) {
    uint32_t crc = spl_crc32(binary, header->loaderSize);

    return crc == header->loaderCrc;
}

#ifndef SPLOADERK
void spl_hdr_setChecksum(SplHeader *header) {
    uint32_t tempcrc = header->loaderCrc;
    header->headerCrc = 0;
    header->loaderCrc = 0;
    header->headerCrc = spl_crc32(&header, sizeof(SplHeader));
    header->loaderCrc = tempcrc;
}
#endif
