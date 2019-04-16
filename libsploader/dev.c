/*
** File: libsploader/dev.c
**
** Author: bringey
**
** Functions for reading/writing to a device (disk, file, etc)
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
