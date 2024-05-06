#include "round.h"
uint8_t forward_s_box[32] = {28, 25, 0, 10, 20, 22, 21, 6, 23, 8, 26, 17, 29, 24, 19, 27, 9,
                             3, 15, 18,1, 31, 2, 11, 12, 4, 30, 5, 16, 7, 13, 14};


void forward_transform_round_function(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
   for (int block_index = 0; block_index < 5; block_index++) {
        uint16_t sum_NMNT[BLOCK_SIZE] = {0}; // Use uint16_t for intermediate sums
        uint8_t data_byte[BLOCK_SIZE] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        printf("Data input:\n");        
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            for (int i = 0; i < BLOCK_SIZE; ++i) {
                printf("data_byte[%d] = %d\n", i, data_byte[i]);
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
            printf("\n");
        }
        
        printf("NMNT Output:\n");
        for (int i = 0; i < BLOCK_SIZE; i++) {
            data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
            printf("data_byte[%d] = %d\n", i, data_byte[i]);

        }
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

void forward_transform(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
    forward_transform_round_function(state, transform_matrix); // Corrected passing of state
}

void split_state_into_data_bytes(ladybug_state_t *state, uint8_t *data_byte, int block_index) {
    // Only work on the specific block at block_index
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        uint8_t byte_offset = 8 * i;
        data_byte[i] = (state->x[block_index] >> byte_offset) & 0xFF;
    }
}

// void combine_data_bytes_to_state(const uint8_t *data_byte, ladybug_state_t *state, int block_index){
//     // Reconstruct state->x[0] from data_byte array in little endian format
//     state->x[block_index] = 0; // Reset state->x[0]
//     for (int i = 0; i < BLOCK_SIZE; i++) {
//         uint64_t temp = data_byte[i];
//         uint8_t byte_offset = 8 * i;
//         state->x[block_index] |= temp << byte_offset;
//     }
// }

void combine_data_bytes_to_state(const uint8_t *data_byte, ladybug_state_t *state, int block_index) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        state -> b[block_index][i] = data_byte[i]; // Set the byte at byte_offset
    }
}

void print_data_byte(ladybug_state_t *state) {
    for (int block_index = 0; block_index < BLOCK_NUMBER; block_index++) {
        printf("BLOCK #%d\n", block_index + 1);
        printf("state: 0x%llx\n", state->x[block_index]);
        uint8_t data_byte[BLOCK_SIZE] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);

        for (int i = 0; i < BLOCK_SIZE; i++) {
            printf("data_byte[%i] = %d\n", i, data_byte[i]);
        }
        printf("\n");
    }
}
