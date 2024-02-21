#include <stdint.h>
#include <string.h>

#include "constants.h"
#include "diffusion_layer.h"
#include "confusion_layer.h"
#include "utils.h"


int main() {
    data_struct s;
    uint8_t data_block_0[N] = "disc0mB0";
    uint8_t data_block_1[N] = "disc0mB1";
    uint8_t data_block_2[N] = "disc0mB2";
    uint8_t data_block_3[N] = "disc0mB3";
    uint8_t data_block_4[N] = "disc0mB4";

    // Load data blocks into state
    memcpy(&s.x[0], data_block_0, sizeof(s.x[0]));
    memcpy(&s.x[1], data_block_1, sizeof(s.x[1]));
    memcpy(&s.x[2], data_block_2, sizeof(s.x[2]));
    memcpy(&s.x[3], data_block_3, sizeof(s.x[3]));
    memcpy(&s.x[4], data_block_4, sizeof(s.x[4]));

    printf("Initial State:\n");
    print_state(s);  // Assuming print_state properly prints the state
    printf("\n");

    uint8_t bundles[64] = {0};

    // Creating 64 5-bit bundles from 5 64-bit blocks
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < BLOCK_NUMBER; j++) {
            bundles[i] |= ((s.x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }

    // Apply Forward S-Box
    for(int i = 0; i < 64; i++) {
        printf("Before S-box:\n");
        printf("bundles[%d] = %d\n", i, bundles[i]);
        print_bitstring(bundles[i], BLOCK_NUMBER);
//        bundles[i] = forward_s_box[bundles[i]]; // Assuming forward_s_box is correctly defined
//        printf("\n");
//        printf("After S-box:\n");
//        printf("bundles[%d] = %d\n", i, bundles[i]);
//        print_bitstring(bundles[i], BLOCK_NUMBER);
        printf("\n");
    }

    // Clear the blocks before reassembling
    memset(s.x, 0, sizeof(s.x));  // Clear existing blocks to ensure accurate reassembly

    // Reassembling 5 64-bit blocks from 64 5-bit bundles
    for(int bit = 0; bit < 64; bit++) {
        for(int block_index = 0; block_index < BLOCK_NUMBER; block_index++) {
            uint64_t bitValue = (bundles[bit] >> (BLOCK_NUMBER - 1 - block_index)) & 0x01;
            s.x[block_index] |= bitValue << (63-bit);
        }
    }

    for(uint8_t i = 0; i < 64; i++) {
        bundles[i] = 0; // Make sure to initialize the bundle
        bundles[i] |= ((s.x[0] >> (63-i)) & 0x1) << 4;
        bundles[i] |= ((s.x[1] >> (63-i)) & 0x1) << 3;
        bundles[i] |= ((s.x[2] >> (63-i)) & 0x1) << 2;
        bundles[i] |= ((s.x[3] >> (63-i)) & 0x1) << 1;
        bundles[i] |= ((s.x[4] >> (63-i)) & 0x1) << 0;
    }

    printf("Final State (Should match initial):\n");
    print_state(s);
    printf("\n");

//    print_state(s);
//    printf("\n");
//    forward_transform(&s, ONMNT, ROUNDS);
//    uint8_t transposedONMNT[N][N];
//    transpose_ONMNT(ONMNT, transposedONMNT);
//    inverse_transform(&s, transposedONMNT, ROUNDS);
    return 0;
}

