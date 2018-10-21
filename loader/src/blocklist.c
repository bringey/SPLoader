
#include <loader/assert.h>
#include <loader/err.h>
#include <loader/blocklist.h>

#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)

// assert that the block is valid (base <= limit)
#define assertblock(block) assert(block.base <= block.limit)


int blocklist_insert(Blocklist *list, Block32 block, bool mergeOverlap) {
    assert(list != NULL);
    assertblock(block);

    if (list->len == list->capacity) {
        return E_FAILURE;
    }

    Block32 *cur;
    bool insert = true;
    size_t i;

    for (i = 0; i != list->len; ++i) {
        cur = list->blocks + i;
        int cmp = blocklist_cmp(block, *cur);
        if (cmp == BLOCKLIST_LESSER) {
            // insert here
            break;
        } else if (cmp == BLOCKLIST_OVERLAPS) {
            if (mergeOverlap) {
                // merge block into cur
                cur->base = min(cur->base, block.base);
                cur->limit = max(cur->limit, block.limit);
                insert = false;
            } else {
                // cannot insert block: overlaps existing block.
                return E_FAILURE;
            }
        }
    }

    if (insert) {
        ++list->len;
        Block32 replace = block;
        Block32 temp;
        for (size_t j = i; j != list->len; ++j) {
            temp = list->blocks[i];
            list->blocks[i] = replace;
            replace = temp;
        }
    }

    return E_SUCCESS;
}

int blocklist_cmp(Block32 b1, Block32 b2) {
    assertblock(b1);
    assertblock(b2);

    if (b1.base > b2.limit) {
        // b1 > b2
        return BLOCKLIST_GREATER;
    } else if (b1.limit < b2.base) {
        // b1 < b2
        return BLOCKLIST_LESSER;
    } else {
        return BLOCKLIST_OVERLAPS;
    }
}