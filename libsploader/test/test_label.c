/*
** File: libsploader/test/test_label.c
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
#include <stdio.h>
#include <string.h>

#include "label.c"

// example device has 1000 total blocks
#define DEV_TOTAL 1000

#define logcall(stmt) do {\
    puts(#stmt); \
    stmt; \
} while (0)

// example partition tables
// A: has an active partition(s) set
// B: no active partitions
// C: some overlapping partition
// D: a partition is out of bounds of the device


// size=1 tables

static SplPart PART_TABLE1A[] = {
    (SplPart){ .num = 0, .active = true, .startLba = 34, .endLba = 245}
};

static SplPart PART_TABLE1B[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = 245}
};

static SplPart PART_TABLE1D[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = DEV_TOTAL+1}
};

// size=5 tables
// only part # 0, 4, 2, 8, 9 exist

static SplPart PART_TABLE5A[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = 245},
    (SplPart){ .num = 4, .active = false, .startLba = 245, .endLba = 300},
    (SplPart){ .num = 2, .active = true, .startLba = 300, .endLba = 301},
    (SplPart){ .num = 8, .active = false, .startLba = 400, .endLba = 500},
    (SplPart){ .num = 9, .active = true, .startLba = 301, .endLba = 400}
};

static SplPart PART_TABLE5B[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = 245},
    (SplPart){ .num = 4, .active = false, .startLba = 245, .endLba = 300},
    (SplPart){ .num = 2, .active = false, .startLba = 300, .endLba = 301},
    (SplPart){ .num = 8, .active = false, .startLba = 400, .endLba = 500},
    (SplPart){ .num = 9, .active = false, .startLba = 301, .endLba = 400}
};

static SplPart PART_TABLE5C[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = 245},
    (SplPart){ .num = 4, .active = false, .startLba = 240, .endLba = 300},
    (SplPart){ .num = 2, .active = false, .startLba = 300, .endLba = 301},
    (SplPart){ .num = 8, .active = false, .startLba = 395, .endLba = 500},
    (SplPart){ .num = 9, .active = false, .startLba = 301, .endLba = 400}
};

static SplPart PART_TABLE5D[] = {
    (SplPart){ .num = 0, .active = false, .startLba = 34, .endLba = 245},
    (SplPart){ .num = 4, .active = false, .startLba = 245, .endLba = 300},
    (SplPart){ .num = 2, .active = false, .startLba = 300, .endLba = 301},
    (SplPart){ .num = 8, .active = false, .startLba = 400, .endLba = DEV_TOTAL+1},
    (SplPart){ .num = 9, .active = false, .startLba = 301, .endLba = 400}
};

static void test_getActive(SplLabel *label, SplPart *table, size_t tablesize, int expreturn, SplPart *expdata) {
    SplPart part;
    SplPart part2;
    memset(&part, 0xFF, sizeof(SplPart));  // so we can test changes to the variable
    memset(&part2, 0xFF, sizeof(SplPart));

    SplLabel testLabel = *label;
    testLabel.table = table;
    testLabel.tablesize = tablesize;
    SplLabel copy = testLabel;
    assert_int_equal(spl_label_getActive(&testLabel, &part), expreturn);
    // test the output variable (unchanged on error, should be expdata on success)
    assert_memory_equal(&part, expreturn == SPL_E_SUCCESS ? expdata : &part2, sizeof(SplPart));
    // test that the label was not modified
    assert_memory_equal(&testLabel, &copy, sizeof(SplLabel));
}


void test_spl_label_getActive(void **state) {

    SplLabel *label = (SplLabel*)(*state);

    // (should fail) test the empty disk label
    logcall(test_getActive(label, NULL, 0, SPL_E_FAILURE, NULL));

    // test the size=1 label with an active partition
    logcall(test_getActive(label, PART_TABLE1A, 1, SPL_E_SUCCESS, PART_TABLE1A));

    // test the size=1 label without an active partition
    logcall(test_getActive(label, PART_TABLE1B, 1, SPL_E_FAILURE, NULL));

    // test the size=5 label with an active partition
    logcall(test_getActive(label, PART_TABLE5A, 5, SPL_E_SUCCESS, PART_TABLE5A + 2));

    // test the size=5 label without an active partition
    logcall(test_getActive(label, PART_TABLE5B, 5, SPL_E_FAILURE, NULL));


}

static void test_getPart(SplLabel *label, SplPart *table, size_t tablesize) {
    SplPart part;
    SplPart part2;
    memset(&part, 0xFF, sizeof(SplPart));  // so we can test changes to the variable
    memset(&part2, 0xFF, sizeof(SplPart));

    SplLabel testLabel = *label;
    testLabel.table = table;
    testLabel.tablesize = tablesize;
    SplLabel copy = testLabel;
    SplPart *testPart;
    for (size_t i = 0; i != tablesize; ++i) {
        testPart = table + i;
        assert_int_equal(spl_label_getPart(&testLabel, testPart->num, &part), SPL_E_SUCCESS);
        assert_memory_equal(&part, testPart, sizeof(SplPart));
        assert_memory_equal(&testLabel, &copy, sizeof(SplLabel));
    }

    // test for failure
    memset(&part, 0xFF, sizeof(SplPart));
    assert_int_equal(spl_label_getPart(&testLabel, UINT32_MAX, &part), SPL_E_FAILURE);
    assert_memory_equal(&part, &part2, sizeof(SplPart));
    assert_memory_equal(&testLabel, &copy, sizeof(SplLabel));

}

void test_spl_label_getPart(void **state) {
    SplLabel *label = (SplLabel*)(*state);

    logcall(test_getPart(label, NULL, 0));
    logcall(test_getPart(label, PART_TABLE1A, 1));
    logcall(test_getPart(label, PART_TABLE5A, 5));

}

static void test_valid(SplLabel *label, SplPart *table, size_t tablesize, int expreturn) {
    SplLabel testLabel = *label;
    testLabel.table = table;
    testLabel.tablesize = tablesize;
    SplLabel copy = testLabel;
    assert_int_equal(spl_label_valid(&testLabel), expreturn);
    assert_memory_equal(&testLabel, &copy, sizeof(SplLabel));
}

void test_spl_label_valid(void **state) {

    SplLabel *label = (SplLabel*)(*state);

    logcall(test_valid(label, NULL, 0, SPL_E_SUCCESS));

    logcall(test_valid(label, PART_TABLE1A, 1, SPL_E_SUCCESS));
    logcall(test_valid(label, PART_TABLE1B, 1, SPL_E_SUCCESS));
    logcall(test_valid(label, PART_TABLE1D, 1, SPL_E_LABEL_BOUNDS));

    logcall(test_valid(label, PART_TABLE5A, 5, SPL_E_SUCCESS));
    logcall(test_valid(label, PART_TABLE5B, 5, SPL_E_SUCCESS));
    logcall(test_valid(label, PART_TABLE5C, 5, SPL_E_LABEL_OVERLAP));
    logcall(test_valid(label, PART_TABLE5D, 5, SPL_E_LABEL_BOUNDS));

}

int setup(void **state) {
    SplDev *dev = (SplDev*)malloc(sizeof(SplDev));
    if (dev == NULL) {
        return EXIT_FAILURE;
    }
    SplLabel *label = (SplLabel*)malloc(sizeof(SplLabel));
    if (label == NULL) {
        return EXIT_FAILURE;
    }
    // "dummy" device
    // only thing we are using from this is the totalBlocks when checking
    // overlapping partitons
    dev->totalBlocks = DEV_TOTAL;
    dev->blocksize = 512;
    dev->flags = 0;
    dev->aux = NULL;
    dev->source = NULL;

    label->dev = dev;
    label->kind = SPL_DISK_LABEL_NONE;
    label->table = NULL;
    label->tablesize = 0;

    *state = label;
    return EXIT_SUCCESS;
}

int teardown(void **state) {
    SplLabel *label = (SplLabel*)(*state);
    free(label->dev);
    free(label);
    *state = NULL;
    return EXIT_SUCCESS;
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_spl_label_getActive),
        cmocka_unit_test(test_spl_label_getPart),
        cmocka_unit_test(test_spl_label_valid)
    };

    return cmocka_run_group_tests(tests, setup, teardown);
}