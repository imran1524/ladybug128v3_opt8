#include "permutations.h"
#include <stdint.h>

//TODO: Add round constant as input parameter

//FORWARD PERMUTATION WITH 3 ROUNDS
 void FP3 (data_struct *s){
    printf("Round # 1\n");
    forward_permutation(s);
    printf("Round # 2\n");
    forward_permutation(s);
//    printf("Round # 3\n");
//    forward_permutation(s);
};

//IP: INVERSE PERMUTATION WITH 3 ROUNDS
void IP3 (data_struct *s){
    printf("Round # 1\n");
    inverse_permutation(s);
    printf("Round # 2\n");
    inverse_permutation(s);
//    printf("Round # 3\n");
//    inverse_permutation(s);
};

//FORWARD PERMUTATION
void forward_permutation(data_struct *s) {
    printf("FORWARD PERMUTATION:\n");
    printf("State before applying forward permutation:\n");
    print_state(s);  // Correctly passing 's' as it's already a pointer
    printf("\n");

    //FORWARD TRANSFORM
    forward_transform(s, NMNT);  // Directly passing 's' since it's a pointer
    printf("State after applying forward transform:\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");

    //Creating 64 5-bit bundles from 5 64-bit blocks
    uint8_t bundles[64] = {0};
    printf("64 5-bit bundle from 5 64-bit blocks:\n");
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }

    printf("Before applying S-box:\n");
    print_bundles(bundles, 64);

    printf("\n");
    //APPLY FORWARD S-BOX
    for(int i = 0; i < 64; i++) {
        bundles[i] = forward_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
    }

    printf("After applying S-box:\n");
    print_bundles(bundles, 64);

    // Clear the blocks before reassembling
    memset(s->x, 0, sizeof(s->x));  // Clear existing blocks to ensure accurate reassembly
    create_blocks_from_bundles(bundles, s);

    for(uint8_t i = 0; i < 64; i++) {
        bundles[i] = 0; // Make sure to initialize the bundle
        bundles[i] |= ((s->x[0] >> (63-i)) & 0x1) << 4;
        bundles[i] |= ((s->x[1] >> (63-i)) & 0x1) << 3;
        bundles[i] |= ((s->x[2] >> (63-i)) & 0x1) << 2;
        bundles[i] |= ((s->x[3] >> (63-i)) & 0x1) << 1;
        bundles[i] |= ((s->x[4] >> (63-i)) & 0x1) << 0;
    }

    printf("State after applying applying forward transform\n");
    print_state(s);
    printf("\n");
}

//INVERSE PERMUTATION
//TODO: Add round constant as input parameter
void inverse_permutation(data_struct *s){
    printf("INVERSE PERMUTATION:\n");
    printf("State before applying inverse S-box\n");
    print_state(s);

    uint8_t bundles[64] = {0};
    // Creating 64 5-bit bundles from 5 64-bit blocks
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s->x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }
    printf("Before applying inverse 5-bit S-box:\n");
    print_bundles(bundles, 64);

    // Apply Inverse S-Box
    for(int i = 0; i < 64; i++) {
          bundles[i] = inverse_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
    }

    printf("\n");
    printf("After applying inverse 5-bit S-box:\n");
    print_bundles(bundles, 64);
    //Reassembling 5 64-bit blocks from 64 5-bit bundles

    create_blocks_from_bundles(bundles, s);

    printf("State before applying inverse transform:\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");

    // INVERSE TRANSFORM
    // TRANSPOSE ONMNT MATRIX
    uint8_t transposedONMNT[N][N];
//    transpose_ONMNT(ONMNT, transposedONMNT);

    inverse_transform(s, NMNT);  // Directly passing 's' since it's a pointer
//    inverse_transform(s, ONMNT, ROUNDS);  // Directly passing 's' since it's a pointer
    printf("State after applying inverse transform:\n");
    print_state(s);  // Correctly passing 's'
    printf("\n");

    char retrievedText[41]; // Enough for 40 characters and a null terminator
    blocks_to_text(s, retrievedText, sizeof(retrievedText));  // Assuming blocks_to_text is updated to take a pointer
    printf("Recovered plaintext:\n");
    for(uint8_t i = 0; i < 40; i++){
        printf("%c", retrievedText[i]);
    }

}