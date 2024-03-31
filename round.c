#include "round.h"
uint8_t forward_s_box[32] = {28, 25, 0, 10, 20, 22, 21, 6, 23, 8, 26, 17, 29, 24, 19, 27, 9,
                             3, 15, 18,1, 31, 2, 11, 12, 4, 30, 5, 16, 7, 13, 14};

uint8_t inverse_s_box[32] = {2, 20, 22, 17, 25, 27, 7, 29, 9, 16, 3, 23, 24, 30, 31, 18, 28,
                             11, 19,14, 4, 6, 5, 8, 13, 1, 10, 15, 0, 12, 26, 21};

//ENCRYPTION ROUND FUNCTION
void forward_transform_round_function(state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
    for(int block_index = 0; block_index < 5; block_index++){
        uint64_t sum_NMNT[BLOCK_SIZE] = {0}; // Initialize sum array to zero
        uint8_t data_byte[BLOCK_SIZE] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            for (int i = 0; i < BLOCK_SIZE; ++i) {
//                printf("data_byte[%d] = %d\n", i, data_byte[i]);
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
//            printf("\n");
        }
        for(int i = 0; i < BLOCK_SIZE; i++){
            data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
        }
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

void forward_transform(state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
    forward_transform_round_function(state, transform_matrix); // Corrected passing of state
}

//DECRYPTION ROUND FUNCTION
void inverse_transform_round_function(state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE], uint8_t inverseN){
    for(int block_index = 0; block_index < BLOCK_NUMBER; block_index++){
        uint64_t sum_NMNT[BLOCK_SIZE] = {0}; // Initialize sum array to zero
        uint8_t data_byte[BLOCK_SIZE] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            for (int i = 0; i < BLOCK_SIZE; ++i) {
//                printf("data_byte[%d] = %d\n", i, data_byte[i]);
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
        }
        for(int i = 0; i < BLOCK_SIZE; i++){
            data_byte[i] = (sum_NMNT[i] * inverseN) % Mp;
        }
        state->x[block_index] = 0;
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

void inverse_transform(state_t *state,  const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
    inverse_transform_round_function(state, transform_matrix,  invN);
}

//TRANSPOSE ONMNT
void transpose_ONMNT(const uint8_t input[BLOCK_SIZE][BLOCK_SIZE], uint8_t output[BLOCK_SIZE][BLOCK_SIZE]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[i][j] = input[j][i];
        }
    }
}
void split_state_into_data_bytes(state_t *state, uint8_t *data_byte, int block_index) {
    // Only work on the specific block at block_index
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        uint8_t byte_offset = 8 * i;
        data_byte[i] = (state->x[block_index] >> byte_offset) & 0xFF;
    }
}

void combine_data_bytes_to_state(const uint8_t *data_byte, state_t *state, int block_index){
    // Reconstruct state->x[0] from data_byte array in little endian format
    state->x[block_index] = 0; // Reset state->x[0]
    for (int i = 0; i < BLOCK_SIZE; i++) {
        uint64_t temp = data_byte[i];
        uint8_t byte_offset = 8 * i;
        state->x[block_index] |= temp << byte_offset;
    }
}

void print_data_byte(state_t *state) {
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
