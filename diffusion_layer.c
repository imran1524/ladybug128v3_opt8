#include "diffusion_layer.h"
#include "constants.h"

//ENCRYPTION ROUND FUNCTION
void forward_transform_round_function(data_struct *state, const uint8_t transform_matrix[N][N]) {
    for(int block_index = 0; block_index < 5; block_index++){
        uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
        }
        for(int i = 0; i < N; i++){
            data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
        }
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

//ENCRYPTION OPERATION
void forward_transform(data_struct *state, const uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        forward_transform_round_function(state, transform_matrix); // Corrected passing of state
    }
}

//DECRYPTION ROUND FUNCTION
void inverse_transform_round_function(data_struct *state, const uint8_t transform_matrix[N][N], uint8_t inverseN){
    for(int block_index = 0; block_index < block_number; block_index++){
        uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
        }
        for(int i = 0; i < N; i++){
            data_byte[i] = (sum_NMNT[i] * inverseN) % Mp;
        }
        state->x[block_index] = 0;
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

//DECRYPTION OPERATION
void inverse_transform(data_struct *state,  const uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        inverse_transform_round_function(state, transform_matrix,  invN);
    }
}

//TRANSPOSE ONMNT
void transpose_ONMNT(const uint8_t input[N][N], uint8_t output[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output[i][j] = input[j][i];
        }
    }
}
void split_state_into_data_bytes(data_struct *state, uint8_t *data_byte, int block_index) {
    // Only work on the specific block at block_index
    for (int i = 0; i < N; ++i) {
        uint8_t byte_offset = 8 * i;
        data_byte[i] = (state->x[block_index] >> byte_offset) & 0xFF;
    }
}

void combine_data_bytes_to_state(uint8_t *data_byte, data_struct *state, int block_index){
    // Reconstruct state->x[0] from data_byte array in little endian format
    state->x[block_index] = 0; // Reset state->x[0]
    for (int i = 0; i < N; i++) {
        uint64_t temp = data_byte[i];
        uint8_t byte_offset = 8 * i;
        state->x[block_index] |= temp << byte_offset;
    }
}



void print_data_byte(data_struct state) {
    for (int block_index = 0; block_index < block_number; block_index++) {
        printf("BLOCK #%d\n", block_index + 1);
        printf("state: 0x%llx\n", state.x[block_index]);
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(&state, data_byte, block_index);

        for (int i = 0; i < N; i++) {
            printf("data_byte[%i] = %d\n", i, data_byte[i]);
        }
        printf("\n");
    }
}
