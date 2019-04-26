/*
** File: libsploader/test/test_dev.c
**
** Author: bringey
*/
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NO_DRIVER_IMPL
#include "dev.c"

// contents of the mock device.
static const uint64_t MOCK_IMAGE[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};


#define MOCK_BLOCKSIZE      sizeof(uint64_t)
#define MOCK_BLOCKCOUNT     (sizeof(MOCK_IMAGE)/sizeof(uint64_t))
#define MOCK_BUFSIZE        sizeof(MOCK_IMAGE)

// mock driver
// just a block of memory (pretend it's a hard drive)

typedef struct MockDrv {
    int error;  // return code if forceError = true
    bool forceError; // if true all functions will return error
    bool forceReadonly; // make this driver readonly
    SplBuf buf;
} MockDrv;

int spl_dev_drv_init(SplDev *dev) {
    MockDrv *drv = (MockDrv*)dev->source;
    dev->blocksize = MOCK_BLOCKSIZE;
    dev->totalBlocks = MOCK_BLOCKCOUNT;
    dev->flags = drv->forceReadonly ? SPL_DEVICE_FLAG_RO : 0;

    return drv->forceError ? drv->error : SPL_E_SUCCESS;
}

int spl_dev_drv_read(SplDev *dev, SplBuf inbuf, uint64_t lba, uint32_t blocks) {
    MockDrv *drv = (MockDrv*)dev->source;
    if (drv->forceError) {
        return drv->error;
    }
    size_t bytes = blocks * dev->blocksize;
    size_t offset = lba * dev->blocksize;
    memcpy(inbuf.loc, drv->buf.loc8 + offset, bytes);
    return SPL_E_SUCCESS;
}

int spl_dev_drv_write(SplDev *dev, SplBuf outbuf, uint64_t lba, uint32_t blocks) {
    MockDrv *drv = (MockDrv*)dev->source;
    if (drv->forceError) {
        return drv->error;
    }
    size_t bytes = blocks * dev->blocksize;
    size_t offset = lba * dev->blocksize;
    memcpy(drv->buf.loc8 + offset, outbuf.loc, bytes);
    return SPL_E_SUCCESS;
}

int mock_setup(void **state) {

    // initialize the mock buffer
    MockDrv *drv = (MockDrv*)malloc(sizeof(MockDrv));
    if (drv == NULL) {
        return EXIT_FAILURE;
    }
    void *buf = malloc(MOCK_BUFSIZE);
    if (buf == NULL) {
        return EXIT_FAILURE;
    }

    drv->forceError = false;
    drv->forceReadonly = false;
    drv->buf.size = MOCK_BUFSIZE;
    drv->buf.loc = buf;

    *state = drv;

    return 0;
}

int mock_teardown(void **state) {
    MockDrv *drv = (MockDrv*)(*state);
    free(drv->buf.loc);
    free(drv);
    *state = NULL;
    return 0;
}


void read_test(SplDev *dev, MockDrv *drv, uint64_t lba, uint32_t blocks, int expReturn) {
    uint64_t readBuf[MOCK_BLOCKCOUNT];
    uint64_t readBufCopy[MOCK_BLOCKCOUNT];
    memset(readBuf, 0xFF, MOCK_BUFSIZE);
    memset(readBufCopy, 0xFF, MOCK_BUFSIZE);
    SplBuf buf = { .size = MOCK_BUFSIZE, .loc64 = readBuf};
    assert_int_equal(spl_dev_read(dev, buf, lba, blocks), expReturn);
    // assert that the mock driver's buffer was not modified
    assert_memory_equal(drv->buf.loc, MOCK_IMAGE, MOCK_BUFSIZE);
    if (expReturn == SPL_E_SUCCESS) {
        // for successful reads, assert that the correct read data was placed
        // in buf, and that the remainder of buf is unmodified
        assert_memory_equal(readBuf, drv->buf.loc64 + lba, blocks * MOCK_BLOCKSIZE);
        assert_memory_equal(readBuf + blocks, readBufCopy, (MOCK_BLOCKCOUNT - blocks) * MOCK_BLOCKSIZE);
    } else {
        // error occurred assert the readBuf is unmodified
        assert_memory_equal(readBuf, readBufCopy, MOCK_BUFSIZE);
    }
}

void write_test(SplDev *dev, MockDrv *drv, uint64_t lba, uint32_t blocks, int expReturn) {
    // image the mock device
    memcpy(drv->buf.loc, MOCK_IMAGE, MOCK_BUFSIZE);
    
    uint64_t writeBuf[MOCK_BLOCKCOUNT];
    uint64_t writeBufCopy[MOCK_BLOCKCOUNT];
    memset(writeBuf, 0xFF, MOCK_BUFSIZE);
    memset(writeBufCopy, 0xFF, MOCK_BUFSIZE);
    SplBuf buf = { .size = MOCK_BUFSIZE, .loc64 = writeBuf};
    
    assert_int_equal(spl_dev_write(dev, buf, lba, blocks), expReturn);
    // assert that the write buffer was not modified
    assert_memory_equal(writeBuf, writeBufCopy, MOCK_BUFSIZE);
    if (expReturn == SPL_E_SUCCESS) {
        // check that the written data is there in the mock buffer
        assert_memory_equal(drv->buf.loc64 + lba, writeBuf, blocks * MOCK_BLOCKSIZE);
        // check that the mock data is unmodified to the left of lba
        assert_memory_equal(drv->buf.loc, MOCK_IMAGE, lba * MOCK_BLOCKSIZE);
        // check that the mock data is unmodified to the right of lba + blocks
        uint32_t right = lba + blocks;
        assert_memory_equal(drv->buf.loc64 + right, MOCK_IMAGE + right, (MOCK_BLOCKCOUNT - right) * MOCK_BLOCKSIZE);
    } else {
        // error occurred, assert that the mock buf is unmodified
        assert_memory_equal(drv->buf.loc, MOCK_IMAGE, MOCK_BUFSIZE);
    }

}


void test_spl_dev_init(void **state) {
    MockDrv *drv = (MockDrv*)(*state);
    SplDev dev = {0};

    // successful return
    drv->forceError = false;
    assert_int_equal(spl_dev_init(&dev, drv), SPL_E_SUCCESS);
    assert_ptr_equal(drv, dev.source);

    // force an error

    // reset dev
    dev = (SplDev){0};
    drv->error = 134;
    drv->forceError = true;
    assert_int_not_equal(spl_dev_init(&dev, drv), SPL_E_SUCCESS);
}

void test_spl_dev_read(void **state) {
    MockDrv *drv = (MockDrv*)(*state);

    // image the mock device
    memcpy(drv->buf.loc, MOCK_IMAGE, MOCK_BUFSIZE);

    // initialize device
    SplDev dev;
    spl_dev_init(&dev, drv);

    drv->forceError = false;

    // read 0 blocks (LBA=0, BLOCKS=0)
    read_test(&dev, drv, 0, 0, SPL_E_SUCCESS);

    // read 1 block (LBA=0, BLOCKS=1)
    read_test(&dev, drv, 0, 1, SPL_E_SUCCESS);

    // read all blocks (LBA=0, BLOCKS=dev.totalBlocks)
    read_test(&dev, drv, 0, dev.totalBlocks, SPL_E_SUCCESS);

    // (should fail) read outside (LBA=0, BLOCKS=dev.totalBlocks+1)
    read_test(&dev, drv, 0, dev.totalBlocks+1, SPL_E_DEV_RANGE);

    // (should fail) read outside (LBA=dev.totalBlocks, BLOCKS=1)
    read_test(&dev, drv, dev.totalBlocks, 1, SPL_E_DEV_RANGE);

    // (should fail) overflow check (LBA=UINTMAX, BLOCKS=1)
    read_test(&dev, drv, UINT64_MAX, 1, SPL_E_DEV_RANGE);

    // (should fail) read 1 block, force error in the mock driver
    drv->forceError = true;
    drv->error = 55;
    read_test(&dev, drv, 1, 1, SPL_E_DEV_READ);
    assert_int_equal(dev.error, drv->error);

}

void test_spl_dev_write(void **state) {
    MockDrv *drv = (MockDrv*)(*state);

    // initialize device
    SplDev dev;
    spl_dev_init(&dev, drv);

    drv->forceError = false;

    // write 0 blocks (LBA=0, BLOCKS=0)
    write_test(&dev, drv, 0, 0, SPL_E_SUCCESS);

    // write 1 block (LBA=0, BLOCKS=1)
    write_test(&dev, drv, 0, 1, SPL_E_SUCCESS);

    // write all blocks (LBA=0, BLOCKS=dev.totalBlocks)
    write_test(&dev, drv, 0, dev.totalBlocks, SPL_E_SUCCESS);

    // (should fail) write outside (LBA=0, BLOCKS=dev.totalBlocks+1)
    write_test(&dev, drv, 0, dev.totalBlocks+1, SPL_E_DEV_RANGE);

    // (should fail) write outside (LBA=dev.totalBlocks, BLOCKS=1)
    write_test(&dev, drv, dev.totalBlocks, 1, SPL_E_DEV_RANGE);

    // (should fail) overflow check (LBA=UINTMAX, BLOCKS=1)
    write_test(&dev, drv, UINT64_MAX, 1, SPL_E_DEV_RANGE);

    // (should fail) write 1 block, force error in the mock driver
    drv->forceError = true;
    drv->error = 55;
    write_test(&dev, drv, 1, 1, SPL_E_DEV_WRITE);
    assert_int_equal(dev.error, drv->error);

    // (should fail) write 1 block, but device is readonly
    SplDev rodev;
    drv->forceReadonly = true;
    spl_dev_init(&rodev, drv);
    drv->forceReadonly = false;
    write_test(&rodev, drv, 1, 1, SPL_E_DEV_READONLY);
}




int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_spl_dev_init),
        cmocka_unit_test(test_spl_dev_read),
        cmocka_unit_test(test_spl_dev_write)
    };

    return cmocka_run_group_tests(tests, mock_setup, mock_teardown);
}