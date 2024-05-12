#include "permutations.h"
#include <stdint.h>

//TODO: Add round constant as input parameter
//FORWARD PERMUTATION WITH 3 ROUNDS

#if !LADYBUG_INLINE_PERM && LADYBUG_UNROLL_LOOPS
    void P1(ladybug_state_t *s){
        //printf("Round # 1\n");
        forward_permutation(s);
    };
#endif

#if 0

#endif

//FORWARD PERMUTATION
void forward_permutation(ladybug_state_t *s) {

    // FORWARD TRANSFORM
    forward_transform(s, NMNT);

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


}

