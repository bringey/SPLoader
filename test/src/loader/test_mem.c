/*
** File: test/src/loader/test_mem.c
**
** Author: bringey
**
** Unit test executable for loader/src/mem.c
*/
#include <loaderstubs.h>

#include "loader/src/mem.c"

size_t _ldr_mem_availableBlocks(void) {
    return 0;
}

size_t _ldr_mem_nextBlock(size_t cont, FreeBlock *block) {
    (void)cont; (void)block;
    return 0;
}


static void test_init(void **state) {
    (void)state;
}

static void test_malloc_(void **state) {
    (void)state;
}

static void test_free_(void **state) {
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init),
        cmocka_unit_test(test_malloc_),
        cmocka_unit_test(test_free_)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}