/*
** File: libsploader/src/sploader.c
**
** Author: bringey
**
*/

#include <sploader.h>

#ifndef SPLOADERK

#ifdef __linux__
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <stdio.h>
#endif


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


uint32_t spl_crc32(void *data, size_t size) {
    // uint32_t byte;
    uint8_t *dp = (uint8_t*)data;
    uint32_t crc = SPL_CRC32_INIT;
    for (size_t i = 0; i != size; ++i) {
        crc = spl_crc32_acc(*dp++, crc);
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

struct SplDev {
    SplDevInfo info;
    void *aux;
};

int spl_dev_init(SplDev *dev, uint32_t forceBs, void *param) {
    SplDevInfo info;
    int err = spl_dev_drv_init(param, forceBs, &info);
    if (err != SPL_E_SUCCESS) {
        return err;
    }

    dev->info = info;
    dev->aux = param;

    return SPL_E_SUCCESS;
}

int spl_dev_info(SplDev *dev, SplDevInfo *info) {
    *info = dev->info;
    return SPL_E_SUCCESS;
}

int spl_dev_pread(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t blocks) {
    (void)dev; (void)buf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

int spl_dev_pwrite(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t blocks) {
    (void)dev; (void)buf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

int spl_dev_read(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t bs, uint32_t blocks) {
    (void)dev; (void)buf; (void)lba; (void)blocks;
    if (bs == 0) {
        return SPL_E_DEV_ZEROBS;
    }

    return SPL_E_FAILURE;
}

int spl_dev_write(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t bs, uint32_t blocks) {
    (void)dev; (void)buf; (void)lba; (void)blocks;
    if (bs == 0) {
        return SPL_E_DEV_ZEROBS;
    }
    return SPL_E_FAILURE;
}


//
// Device Driver for tooling (only works for linux atm)
//

#ifndef SPLOADERK
// default dev driver for the tooling (underlying device is a FILE*)

int spl_dev_drv_init(void *aux, uint32_t forceBs, SplDevInfo *info) {

#ifdef __linux__

    FILE *fp = (FILE*)aux;
    uint32_t blocksize = 512; // default to 512
    int fd = fileno(fp);
    if (fd < 0) {
        return SPL_E_DEV_BADFILE;
    }
    

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        return SPL_E_DEV_BADFILE;
    }

    if (S_ISBLK(statbuf.st_mode)) {
        // underlying file is a block device
        // use ioctl to get the total blocks and physical blocksize
        uint64_t sizeInBytes;
        if (ioctl(fd, BLKGETSIZE64, &sizeInBytes) < 0) {
            return SPL_E_DEV_BADFILE;
        }
        if (forceBs == SPL_DEVICE_BS) {
            if (ioctl(fd, BLKPBSZGET, &blocksize) < 0) {
                return SPL_E_DEV_BADFILE;
            }
        }
        uint64_t blocks = sizeInBytes / blocksize;
        if (sizeInBytes % blocksize != 0) {
            return SPL_E_DEV_BSALIGN; // blocksize is not aligned
        }
        info->totalBlocks = blocks;
    }

    if (forceBs) {
        blocksize = forceBs;
    }

    info->blocksize = blocksize;
    info->flags = 0;
    info->maxBlocksPerRead = 0;
    info->maxBlocksPerWrite = 0;

    return SPL_E_SUCCESS;

#else

    (void)aux; (void)forceBs; (void)info;
    return SPL_E_FAILURE;

#endif
}

int spl_dev_drv_read(void *aux, SplBuf inBuf, uint64_t lba, uint32_t blocks) {
    (void)aux; (void)inBuf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

int spl_dev_drv_write(void *aux, SplBuf outBuf, uint64_t lba, uint32_t blocks) {
    (void)aux; (void)outBuf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

#endif
