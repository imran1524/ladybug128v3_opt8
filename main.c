#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "diffusion_layer.h"
#include "confusion_layer.h"
#include "utils.h"

int main() {
    //PLAINTEXT
    data_struct s;
    data_struct t;
    uint8_t data_block_0[N] = "disc0mB0"; //Message block 1
    uint8_t data_block_1[N] = "disc0mB1"; //Message block 2
    uint8_t data_block_2[N] = "disc0mB2"; //Message block 3
    uint8_t data_block_3[N] = "disc0mB3"; //Message block 4
    uint8_t data_block_4[N] = "disc0mB4"; //Message block 5

    //LOAD DATA BLOCK FROM STATE
    memcpy(&s.x[0], data_block_0, sizeof(s.x[0]));
    memcpy(&s.x[1], data_block_1, sizeof(s.x[1]));
    memcpy(&s.x[2], data_block_2, sizeof(s.x[2]));
    memcpy(&s.x[3], data_block_3, sizeof(s.x[3]));
    memcpy(&s.x[4], data_block_4, sizeof(s.x[4]));

    uint64_t bundles[64]; // There will be 64 5-bit bundles

    // Initialize bundles to 0
    for(int i = 0; i < 64; i++) {
        bundles[i] = 0;
    }

    // Create bundles
    for(int i = 0; i < 64; i++) { // For each bit position in the 64-bit vectors
        for(int j = 0; j < BLOCK_NUMBER; j++) { // For each of the 5 vectors
            // Shift the j-th bit of the i-th bundle into position, and OR it with the existing bundle value
//            printf("%llx\n", s.x[j]);
            bundles[i] |= ((s.x[j] >> (63-i)) & 0x1) << (BLOCK_NUMBER - 1 - j);
        }
    }
    printf("Forward S-box\n");
    print_vector(bundles, 64);
    for(int bundle_index = 0; bundle_index < 64; bundle_index++){
        printf("Forward S-box: bundle[%d] = %d\n", bundle_index, forward_s_box[bundles[bundle_index]]);
    }
    printf("\n");
    printf("Inverse S-box\n");
    for(int bundle_index = 0; bundle_index < 64; bundle_index++){
        printf("Inverse S-box: bungle[%d] = %d\n", bundle_index, inverse_s_box[forward_s_box[bundles[bundle_index]]]);
    }

    //LINEAR LAYER
    //print_state(s);
    //print_data_byte(s);
    forward_transform(&s, ONMNT, ROUNDS);
    //print_state(s);
    //print_data_byte(s);    //NON-LINEAR LAYER

    //printf("RECOVERED PLAINTEXT:\n");
    //TRANSPOSE MATRIX ONLY FOR ONMNT
    uint8_t transposedONMNT[N][N];
    transpose_ONMNT(ONMNT, transposedONMNT);
    //print_state(s);
    //print_data_byte(s);
    inverse_transform(&s, transposedONMNT, ROUNDS);
    //print_state(s);
    //print_data_byte(s);

    return 0;
}




