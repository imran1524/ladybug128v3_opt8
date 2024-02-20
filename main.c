#include <stdio.h>
#include <stdint.h> //
#include <string.h>

#include "diffusion_layer.h"
#include "constants.h"

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

    //LINEAR LAYER
    printf("PLAINTEXT:\n");

    print_state(s);
    print_data_byte(s);
    forward_transform(&s, ONMNT, ROUNDS);
    print_state(s);
    print_data_byte(s);    //NON-LINEAR LAYER

    printf("RECOVERED PLAINTEXT:\n");
    //TRANSPOSE MATRIX ONLY FOR ONMNT
    uint8_t transposedONMNT[N][N];
    transpose_ONMNT(ONMNT, transposedONMNT);
    print_state(s);
    print_data_byte(s);
    inverse_transform(&s, transposedONMNT, ROUNDS);
    print_state(s);
    print_data_byte(s);

    return 0;
}




