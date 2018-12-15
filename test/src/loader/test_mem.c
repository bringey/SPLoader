/*
** File: test/src/loader/test_mem.c
**
** Author: bringey
**
** Unit test executable for loader/src/mem.c
*/
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <loader/mem.h>


size_t _mem_availableBlocks(void) {
    return 0;
}

size_t _mem_nextBlock(size_t cont, FreeBlock *block) {
    (void)cont; (void)block;
    return 0;
}


static void test_init(void **state) {
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}