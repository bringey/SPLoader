/*
** File: loader/test/src/test_string.c
**
** Author: bringey
**
*/


#include <loaderstubs.h>
#include <string.h>

#include "string.c"

#define ARR_FOREACH(arr, var) for (size_t var = 0; var != sizeof(arr)/sizeof(*arr); ++var)

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

    struct testcase {
        char *input;
        const char *expected;
    };

    static struct testcase cases[] = {
        {"", ""},
        {"A", "a"},
        {"b", "b"},
        {"Test", "test"},
        {"alreadylower", "alreadylower"},
        {"TEST", "test"},
        {"\a\a\tABcD\a", "\a\a\tabcd\a"},
        {"123FOO", "123foo"}
    };

    //expect_assert();
    //expect_except(ldr_strlower(NULL));
    char buf[80];
    struct testcase *cur = cases;
    ARR_FOREACH(cases, i) {
        strcpy(buf, cur->input);
        int err = ldr_strlower(buf);
        assert_int_equal(err, E_SUCCESS);
        assert_string_equal(buf, cur->expected);
        ++cur;
    }

}

static void test_ldr_strlen(void **state) {
    (void)state;

    struct strlen_testcase {
        const char *input;
        size_t expected;
    };

    static struct strlen_testcase cases[] = {
        {"", 0},
        {"a", 1},
        {"foobar", 6}
    };

    struct strlen_testcase *cur = cases;
    //for (size_t i = 0; i != sizeof(cases)/sizeof(struct strlen_testcase); ++i) {
    ARR_FOREACH(cases, i) {
        assert_int_equal(ldr_strlen(cur->input), cur->expected);
        ++cur;
    }

    // #define strlen_test(str, expected) do { \
    //     const char *input = str; \
    //     assert_int_equal(ldr_strlen(input), expected); \
    // } while (0)

    // // empty string
    // strlen_test("", 0);
    // strlen_test("a", 1);
    // strlen_test("foobar", 6);

    // #undef strlen_test
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
