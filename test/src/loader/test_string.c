/*
** File: test/src/loader/test_string.c
**
** Author: bringey
**
*/


#include <loaderstubs.h>

#include "loader/src/string.c"

static void test_ltostr(void **state) {
    (void)state;
}

static void test_ultostr(void **state) {
    (void)state;
}

static void test_memcpy(void **state) {
    (void)state;
}

static void test_strupper(void **state) {
    (void)state;
}

static void test_strlower(void **state) {
    (void)state;
}

static void test_strlen(void **state) {
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ltostr),
        cmocka_unit_test(test_ultostr),
        cmocka_unit_test(test_memcpy),
        cmocka_unit_test(test_strupper),
        cmocka_unit_test(test_strlower),
        cmocka_unit_test(test_strlen)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
