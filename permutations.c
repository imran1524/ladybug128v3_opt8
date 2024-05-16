#include "permutations.h"
#include <stdint.h>
uint8_t forward_s_box[32] = {28, 25, 0, 10, 20, 22, 21, 6, 23, 8, 26, 17, 29, 24, 19, 27, 9,
                             3, 15, 18,1, 31, 2, 11, 12, 4, 30, 5, 16, 7, 13, 14};

//TODO: Add round constant as input parameter
//FORWARD PERMUTATION WITH 3 ROUNDS

#if !LADYBUG_INLINE_PERM && LADYBUG_UNROLL_LOOPS
    void P1(ladybug_state_t *s){
        forward_permutation(s);
    };
#endif

#if 0

#endif

//FORWARD PERMUTATION
void forward_permutation(ladybug_state_t *s) {
    uint8_t bundles[64] = {0};

    // Creating 64 5-bit bundles from 5 8-bit blocks
    for(size_t i = 0; i < 64; i++) {
        for(size_t j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->b[j][i / 8] >> (7 - (i % 8))) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }

    // APPLY FORWARD S-BOX
    for(size_t i = 0; i < 64; i++) {
        bundles[i] = forward_s_box[bundles[i]];
    }

    // Clear the blocks before reassembling
    memset(s->b, 0, sizeof(s->b));

    // Create blocks from bundles
    for(size_t i = 0; i < 64; i++) {
        for(size_t j = 0; j < BLOCK_NUMBER; j++) {
            s->b[j][i / 8] |= ((bundles[i] >> (BLOCK_NUMBER - 1 - j)) & 0x1) << (7 - (i % 8));
        }
    }
    // FORWARD TRANSFORM
    // printf("Print state before forward transform:\n");
    // print_data_byte(s);
    forward_transform(s, NMNT);
    // printf("Print state after forward transform:\n");
    // print_data_byte(s);
}

