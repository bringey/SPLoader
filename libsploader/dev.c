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
#include <errno.h>
#endif

#include <stdio.h>
#endif


int spl_dev_init(SplDev *dev, void *param) {
    dev->source = param;
    int err = spl_dev_drv_init(dev);
    if (err != SPL_E_SUCCESS) {
        return err;
    }

    return SPL_E_SUCCESS;
}

int spl_dev_read(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t blocks) {

    if (blocks) {
        // check the range of lba + blocks
        if (lba > UINT64_MAX - blocks || lba + blocks > dev->totalBlocks) {
            return SPL_E_DEV_RANGE;
        }
        // make sure the buffer is big enough
        if (buf.size < blocks * dev->blocksize) {
            return SPL_E_DEV_BUFSIZE;
        }

        int err = spl_dev_drv_read(dev, buf, lba, blocks);
        if (err) {
            dev->error = err;
            return SPL_E_DEV_READ;
        }
    }

    return SPL_E_SUCCESS;
}

int spl_dev_write(SplDev *dev, SplBuf buf, uint64_t lba, uint32_t blocks) {

    if (dev->flags & SPL_DEVICE_FLAG_RO) {
        // device source does not support writes (readonly)
        return SPL_E_DEV_READONLY;
    }

    if (blocks) {
        // check the range of lba + blocks
        if (lba > UINT64_MAX - blocks || lba + blocks > dev->totalBlocks) {
            return SPL_E_DEV_RANGE;
        }
        // make sure the buffer is big enough
        if (buf.size < blocks * dev->blocksize) {
            return SPL_E_DEV_BUFSIZE;
        }

        int err = spl_dev_drv_write(dev, buf, lba, blocks);
        if (err) {
            dev->error = err;
            return SPL_E_DEV_WRITE;
        }
    }

    return SPL_E_SUCCESS;
}


//
// Device Driver for tooling (only works for linux atm)
//

#ifndef NO_DRIVER_IMPL


#ifdef __linux__

// default dev driver for the tooling (underlying device is a FILE*)

int spl_dev_drv_init(SplDev *dev) {

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
        if (ioctl(fd, BLKPBSZGET, &blocksize) < 0) {
            return SPL_E_DEV_BADFILE;
        }

        uint64_t blocks = sizeInBytes / blocksize;
        if (sizeInBytes % blocksize != 0) {
            return SPL_E_DEV_BSALIGN; // blocksize is not aligned
        }
        dev->totalBlocks = blocks;
    }

    dev->blocksize = blocksize;
    dev->flags = 0;

    return SPL_E_SUCCESS;
}

int spl_dev_drv_read(SplDev *dev, SplBuf inBuf, uint64_t lba, uint32_t blocks) {
    int fd = fileno((FILE*)dev->source);
    if (fd < 0) {
        dev->error = errno;
        return SPL_E_DEV_READ;
    }
    if (blocks) {
        size_t bytes = blocks * dev->blocksize;
        off_t offset = lba * dev->blocksize;
        ssize_t nread = pread(fd, inBuf.loc, bytes, offset);
        if (nread < 0) {
            dev->error = errno;
            return SPL_E_DEV_READ;
        }
        if ((size_t)nread != bytes) {
            // read was incomplete
            return SPL_E_DEV_READ;
        }
    }
    return SPL_E_SUCCESS;
}

int spl_dev_drv_write(SplDev *dev, SplBuf outBuf, uint64_t lba, uint32_t blocks) {
    int fd = fileno((FILE*)dev->source);
    if (fd < 0) {
        dev->error = errno;
        return SPL_E_DEV_WRITE;
    }
    if (blocks) {
        size_t bytes = blocks * dev->blocksize;
        off_t offset = lba * dev->blocksize;
        ssize_t nwritten = pwrite(fd, outBuf.loc, bytes, offset);
        if (nwritten < 0) {
            dev->error = errno;
            return SPL_E_DEV_WRITE;
        }
        if ((size_t)nwritten != bytes) {
            // write was incomplete
            return SPL_E_DEV_WRITE;
        }
    }
    
    return SPL_E_SUCCESS;
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
