/*
** File: loader/test/test_mem.c
**
** Author: bringey
**
** Unit test executable for loader/src/mem.c
*/
#include <loaderstubs.h>

#include "mem.c"

size_t _ldr_mem_availableBlocks(void) {
    return 0;
}

size_t _ldr_mem_nextBlock(size_t cont, FreeBlock *block) {
    (void)cont; (void)block;
    return 0;
}


static void test_ldr_mem_init(void **state) {
    (void)state;
}

static void test_ldr_malloc(void **state) {
    (void)state;
}

static void test_ldr_free(void **state) {
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ldr_mem_init),
        cmocka_unit_test(test_ldr_malloc),
        cmocka_unit_test(test_ldr_free)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}