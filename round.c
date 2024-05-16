#include "round.h"

void forward_transform_round_function(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
   for (int block_index = 0; block_index < 5; block_index++) {
        uint16_t sum_NMNT[BLOCK_SIZE] = {0}; // Use uint16_t for intermediate sums
        uint8_t data_byte[BLOCK_SIZE] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        // printf("Data input for Block # %d\n", block_index + 1);        
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            // printf("data_byte[%d] = %d\n", j, data_byte[j]);
            for (int i = 0; i < BLOCK_SIZE; ++i) {
                // printf("data_byte[%d] = %d\n", i, data_byte[i]);
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
            // printf("\n");
        }
        
        // printf("NMNT Output:\n");
        for (int i = 0; i < BLOCK_SIZE; i++) {
            data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
            // printf("data_byte[%d] = %d\n", i, data_byte[i]);
        }
        // printf("\n");
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

void forward_transform(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]) {
    // printf("Calling forward_transform for state forward_transform\n");
    // printf("State before forward_transform_round_function\n");
    // print_data_byte(state);
    forward_transform_round_function(state, transform_matrix); // Corrected passing of state
    // printf("State after forward_transform_round_function\n");
    // print_data_byte(state);
}

void split_state_into_data_bytes(ladybug_state_t *state, uint8_t *data_byte, int block_index) {
    //Null pointer check
    if(state == NULL || data_byte == NULL) {
        fprintf(stderr, "Error: Null pointer passed to split_state_into_data_bytes\n");
        return;
    }

    // Boundary check
    if (block_index < 0 || block_index >= BLOCK_NUMBER) {
        fprintf(stderr, "Error: Invalid block_index passed to split_state_into_data_bytes\n");
        return;
    }

    // Convert the 64-bit value to an array of 8-bit values
    uint64_t value = state->x[block_index];
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        data_byte[i] = (value >> (i * 8)) & 0xFF;
    }

}

void combine_data_bytes_to_state(const uint8_t *data_byte, ladybug_state_t *state, int block_index) {
    
    // Null pointer check
    if (state == NULL || data_byte == NULL) {
        fprintf(stderr, "Error: Null pointer passed to combine_data_bytes_to_state\n");
        return;
    }

    // Boundary check
    if (block_index < 0 || block_index >= BLOCK_NUMBER) {
        fprintf(stderr, "Error: Invalid block_index passed to combine_data_bytes_to_state\n");
        return;
    }

      // Convert the array of 8-bit values back to a 64-bit value
    uint64_t value = 0;
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        value |= ((uint64_t)data_byte[i] << (i * 8));
    }
    state->x[block_index] = value;
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

