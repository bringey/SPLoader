/*
** File: loader/test/src/test_string.c
**
** Author: bringey
**
*/


#include <loaderstubs.h>

#include "string.c"

static void test_ldr_ltostr(void **state) {
    (void)state;
}

static void test_ldr_ultostr(void **state) {
    (void)state;
}

static void test_ldr_memcpy(void **state) {
    (void)state;
}

static void test_ldr_strupper(void **state) {
    (void)state;
}

static void test_ldr_strlower(void **state) {
    (void)state;
}

static void test_ldr_strlen(void **state) {
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ldr_ltostr),
        cmocka_unit_test(test_ldr_ultostr),
        cmocka_unit_test(test_ldr_memcpy),
        cmocka_unit_test(test_ldr_strupper),
        cmocka_unit_test(test_ldr_strlower),
        cmocka_unit_test(test_ldr_strlen)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
