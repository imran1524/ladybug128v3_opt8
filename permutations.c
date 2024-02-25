#include "permutations.h"

void forward_permutation(data_struct *s) {
    printf("Initial state at forward permutation:\n");
    print_state(s);  // Correctly passing 's' as it's already a pointer
    printf("\n");


#if 0
    uint8_t bundles[64] = {0};

    //Creating 64 5-bit bundles from 5 64-bit blocks
    printf("64 5-bit bundle from 5 64-bit blocks:\n");
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
        printf("bundles[%d] = %d\n", i, bundles[i]);
        print_bitstring(bundles[i], BLOCK_NUMBER);
        printf("\n");
    }

    for(int i = 0; i < 64; i++){
        printf("bundle[%d] = %d\n", i, bundles[i]);
    }

    printf("\n");
    // Apply Forward S-Box
    for(int i = 0; i < 64; i++) {
//        printf("Before S-box:\n");
//        printf("bundles[%d] = %d\n", i, bundles[i]);
//        print_bitstring(bundles[i], BLOCK_NUMBER);
        bundles[i] = forward_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
//        printf("\n");
//        printf("After S-box:\n");
//        printf("bundles[%d] = %d\n", i, bundles[i]);
        print_bitstring(bundles[i], BLOCK_NUMBER);
        printf("\n");
    }

    // Clear the blocks before reassembling
    memset(s->x, 0, sizeof(s->x));  // Clear existing blocks to ensure accurate reassembly

    // Reassembling 5 64-bit blocks from 64 5-bit bundles
    for(int bit = 0; bit < 64; bit++) {
        for(int block_index = 0; block_index < BLOCK_NUMBER; block_index++) {
            uint64_t bitValue = (bundles[bit] >> (BLOCK_NUMBER - 1 - block_index)) & 0x01;
            s->x[block_index] |= bitValue << (63-bit);
        }
    }

    for(uint8_t i = 0; i < 64; i++) {
        bundles[i] = 0; // Make sure to initialize the bundle
        bundles[i] |= ((s->x[0] >> (63-i)) & 0x1) << 4;
        bundles[i] |= ((s->x[1] >> (63-i)) & 0x1) << 3;
        bundles[i] |= ((s->x[2] >> (63-i)) & 0x1) << 2;
        bundles[i] |= ((s->x[3] >> (63-i)) & 0x1) << 1;
        bundles[i] |= ((s->x[4] >> (63-i)) & 0x1) << 0;
    }

    printf("State before applying forward transform:\n");
    print_state(s);

//    for(int i = 0; i < BLOCK_NUMBER; i++){
//        for(int j = 0; j < N; j++){
//            printf("s.x[%d] = %d\n");
//        }
//    }

    printf("\n");
#endif


    // FORWARD TRANSFORM
    forward_transform(s, ONMNT, ROUNDS);  // Directly passing 's' since it's a pointer
    printf("State after applying forward transform:\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");
}

void inverse_permutation(data_struct *s){
    printf("Initial state at inverse permutation\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");

    // INVERSE TRANSFORM
    // TRANSPOSE ONMNT MATRIX
    uint8_t transposedONMNT[N][N];
    transpose_ONMNT(ONMNT, transposedONMNT);

    inverse_transform(s, transposedONMNT, ROUNDS);  // Directly passing 's' since it's a pointer
    printf("State after applying inverse transform:\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");


#if 0
    uint8_t bundles[64] = {0};
    // Creating 64 5-bit bundles from 5 64-bit blocks
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }

    // Apply Forward S-Box
    for(int i = 0; i < 64; i++) {
        printf("Before applying inverse 5-bit S-box:\n");
        printf("bundles[%d] = %d\n", i, bundles[i]);
        print_bitstring(bundles[i], BLOCK_NUMBER);
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < 64; i++) {
        printf("After applying inverse 5-bit S-box:\n");
        bundles[i] = inverse_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
        printf("bundles[%d] = %d\n", i, bundles[i]);
        print_bitstring(bundles[i], BLOCK_NUMBER);
        printf("\n\n");
    }
    printf("\n");
    // Reassembling 5 64-bit blocks from 64 5-bit bundles
    for(int bit = 0; bit < 64; bit++) {
        for(int block_index = 0; block_index < BLOCK_NUMBER; block_index++) {
            uint64_t bitValue = (bundles[bit] >> (BLOCK_NUMBER - 1 - block_index)) & 0x01;
            s->x[block_index] |= bitValue << (63-bit);
        }
    }

#endif
    char retrievedText[41]; // Enough for 40 characters and a null terminator
    blocks_to_text(s, retrievedText, sizeof(retrievedText));  // Assuming blocks_to_text is updated to take a pointer
    printf("Retrieved Text: %s\n", retrievedText);
}