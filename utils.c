#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "constants.h"

void print_vector(uint8_t *vector, int vector_size){
    for(uint8_t i = 0; i < vector_size; i++){
        printf("vector[%d] = %u\n", i, vector[i]);
    }
}

void print_state(data_struct state){
    for(int i = 0; i < BLOCK_NUMBER; i++){
        printf("state.x[%d] = %llx\n", i, state.x[i]);
    }
}

void print_bitstring(uint8_t vector, uint8_t bit_number) {
    for(int8_t bit_index = bit_number - 1; bit_index >= 0; bit_index--) {
        printf("%d", (vector >> bit_index) & 0x01);
    }
}

