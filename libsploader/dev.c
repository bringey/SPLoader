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
#include <unistd.h>
#endif

#include <stdio.h>
#endif


int spl_dev_init(SplDev *dev, uint32_t forceBs, void *param) {
    dev->source = param;
    int err = spl_dev_drv_init(dev, forceBs);
    if (err != SPL_E_SUCCESS) {
        return err;
    }

    return SPL_E_SUCCESS;
}

// int spl_dev_info(SplDev *dev, SplDevInfo *info) {
//     *info = dev->info;
//     return SPL_E_SUCCESS;
// }

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


#ifdef __linux__

// default dev driver for the tooling (underlying device is a FILE*)

int spl_dev_drv_init(SplDev *dev, uint32_t forceBs) {

    FILE *fp = (FILE*)dev->source;
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
        dev->totalBlocks = blocks;
    }

    if (forceBs) {
        blocksize = forceBs;
    }

    dev->blocksize = blocksize;
    dev->flags = 0;

    return SPL_E_SUCCESS;
}

int spl_dev_drv_read(SplDev *dev, SplBuf inBuf, uint64_t lba, uint32_t blocks) {
    //FILE *fp = (FILE*)dev->aux;
    int fd = fileno((FILE*)dev->source);
    if (blocks) {
        size_t bytes = blocks * dev->blocksize;
        off_t offset = lba * dev->blocksize;
        ssize_t nread = pread(fd, inBuf.loc, bytes, offset);
        if (nread < 0 || (size_t)nread != bytes) {
            // an error occurred during the read, or the read was incomplete
            return SPL_E_DEV_READ;
        }
    }
    return SPL_E_SUCCESS;
}

int spl_dev_drv_write(SplDev *dev, SplBuf outBuf, uint64_t lba, uint32_t blocks) {
    (void)dev; (void)outBuf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

#else

int spl_dev_drv_init(SplDev *dev, uint32_t forceBs) {
    (void)dev; (void)forceBs;
    return SPL_E_FAILURE;
}

int spl_dev_drv_read(SplDev *dev, SplBuf inBuf, uint64_t lba, uint32_t blocks) {
    (void)dev; (void)inBuf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}


int spl_dev_drv_write(SplDev *dev, SplBuf outBuf, uint64_t lba, uint32_t blocks) {
    (void)dev; (void)outBuf; (void)lba; (void)blocks;
    return SPL_E_FAILURE;
}

#endif


#endif
