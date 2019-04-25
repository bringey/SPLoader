/*
** File: libsploader/test/test_util.c
**
** Author: bringey
*/
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "util.c"

#define TEST_BUF_SIZE 32

static uint8_t zeros[TEST_BUF_SIZE];
static uint8_t ones[TEST_BUF_SIZE];
static uint8_t incrementing[TEST_BUF_SIZE];
static uint8_t decrementing[TEST_BUF_SIZE];


static void test_spl_crc32(void **state) {
    (void)state;

    // this test uses test vectors from rfc3720 appendix-B.4
    // however, expected values do not match since that rfc is for crc32c

    static uint8_t* tests[] = { zeros, ones, incrementing, decrementing};
    static uint32_t expecteds[] = {
        0x190A55AD, 0xFF6CAB0B, 0x91267E8A, 0x9AB0EF72
    };

    for (int i = 0; i != (sizeof(tests) / sizeof(uint8_t*)); ++i) {
        assert_int_equal(expecteds[i], spl_crc32(tests[i], TEST_BUF_SIZE));
    }

    // check result from the crc catalog
    static char* check = "123456789";
    assert_int_equal(0xCBF43926, spl_crc32(check, 9));

}


int main(void) {
    for (int i = 0; i != TEST_BUF_SIZE; ++i) {
        zeros[i] = 0;
        ones[i] = 0xFF;
        incrementing[i] = i;
        decrementing[i] = TEST_BUF_SIZE - i - 1;
    }

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_spl_crc32)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}