#include "permutations.h"
#include <stdint.h>

//TODO: Add round constant as input parameter
//FORWARD PERMUTATION WITH 3 ROUNDS

#if !LADYBUG_INLINE_PERM && LADYBUG_UNROLL_LOOPS
     void FP1(ladybug_state_t *s){
//    printf("Round # 1\n");
    forward_permutation(s);
};
#endif


//IP: INVERSE PERMUTATION WITH 3 ROUNDS
void IP1(ladybug_state_t *s){
    // inverse_permutation(s);At 
};

//FORWARD PERMUTATION
void forward_permutation(ladybug_state_t *s) {

    //Creating 64 5-bit bundles from 5 64-bit blocks
    uint8_t bundles[64] = {0};

//    printf("64 5-bit bundle from 5 64-bit blocks:\n");
    for(size_t i = 0; i < 64; i++) {
        for(size_t j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }

//    printf("Before applying S-box:\n");
//    print_bundles(bundles, 64);
//    printf("\n");
    //APPLY FORWARD S-BOX
    for(size_t i = 0; i < 64; i++) {
        bundles[i] = forward_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
    }

//    printf("After applying S-box:\n");
//    print_bundles(bundles, 64);

    // Clear the blocks before reassembling
    memset(s->x, 0, sizeof(s->x));  // Clear existing blocks to ensure accurate reassembly
    create_blocks_from_bundles(bundles, s);

    /**
     * Converts the bits of each element in the array `s->x` into bundles of bits.
     * Each bundle is stored in the corresponding element of the `bundles` array.
     * The bundles are created by taking the bits from `s->x` in reverse order and
     * placing them in the appropriate positions in the bundle.
     *
     * @param s Pointer to a structure containing the array `s->x`
     * @param bundles Array to store the bundles of bits
     */
    for(size_t i = 0; i < 64; i++) {
        bundles[i] = 0; // Make sure to initialize the bundle
        bundles[i] |= ((s->x[0] >> (63-i)) & 0x1) << 4;
        bundles[i] |= ((s->x[1] >> (63-i)) & 0x1) << 3;
        bundles[i] |= ((s->x[2] >> (63-i)) & 0x1) << 2;
        bundles[i] |= ((s->x[3] >> (63-i)) & 0x1) << 1;
        bundles[i] |= ((s->x[4] >> (63-i)) & 0x1) << 0;
    }

//    printf("State after applying applying forward transform\n");
//    print_state(s);
//    printf("\n");

//    printf("FORWARD PERMUTATION:\n");
//    printf("State before applying forward permutation:\n");
//    print_state(s);  // Correctly passing 's' as it's already a pointer
//    printf("\n");

    //FORWARD TRANSFORM
    forward_transform(s, NMNT);  // Directly passing 's' since it's a pointer
//    printf("State after applying forward transform:\n");
//    print_state(s);  // Correctly passing 's'
//    printf("\n");
}

