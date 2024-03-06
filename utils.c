#include <stdio.h>
#include "utils.h"
#include "constants.h"
#include "aead.h"

void print_vector(uint8_t *vector, int vector_size){
    for(uint8_t i = 0; i < vector_size; i++){
        printf("vector[%d] = %u\n", i, vector[i]);
    }
}

void print_bundles(uint8_t *bundle, uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        printf("bundle[%d] = %d\n", i, bundle[i]);
    }
}

void print_state(state_t *state) {
    for (int i = 0; i < BLOCK_NUMBER; i++) {
        printf("state.x[%d] = ", i);
        for (int bit = 63; bit >= 0; bit--) {
            // Shift the bit to the least significant position and mask with 1
            printf("%llu ", (state->x[i] >> bit) & 0x1);
        }
        printf("\n");
    }
}

void print_bitstring(uint8_t vector, uint8_t bit_number) {
    for(int8_t bit_index = bit_number - 1; bit_index >= 0; bit_index--) {
        printf("%d\n", (vector >> bit_index) & 0x01);
    }
}

// Function to convert text to 64-bit blocks and store in data_struct
void text_to_64bit_blocks_data_struct(const char* text, state_t* s) {
    size_t maxTextLength = BLOCK_NUMBER * 8; // Max text length to fit in 5 x 64-bit blocks
    size_t textLength = strlen(text);

    if (textLength > maxTextLength) {
        textLength = maxTextLength; // Truncate text to fit in 320 bits
    }
    printf("Plaintext:\n");
    for(uint8_t i = 0; i < maxTextLength; i++){
        printf("%c", text[i]);
    }
    printf("\n");
    // Initialize blocks to 0
    memset(s->x, 0, BLOCK_NUMBER * sizeof(uint64_t));

    // Pack characters into 64-bit blocks
    for (size_t i = 0; i < textLength; ++i) {
        size_t blockIndex = i / 8; // Determine which block the character belongs to
        size_t charIndex = i % 8; // Determine the position within the block
        ((uint8_t*)(&s->x[blockIndex]))[charIndex] = (uint8_t)text[i]; // Pack character
    }
}

// Function to combine the data from data_struct and get the text back
void blocks_to_text(state_t* s, char* outText, size_t maxTextLength) {
    size_t byteIndex = 0;
    for (size_t i = 0; i < BLOCK_NUMBER; ++i) {
        for (size_t j = 0; j < 8; ++j) { // 8 bytes per block
            if (byteIndex < maxTextLength - 1) { // Leave space for null terminator
                uint8_t currentByte = (s->x[i] >> (j * 8)) & 0xFF;
                outText[byteIndex++] = currentByte;
            } else {
                break; // Stop if there's no more space in the output buffer
            }
        }
    }
    outText[byteIndex] = '\0'; // Null-terminate the output string
}

void create_blocks_from_bundles(uint8_t bundles[64], state_t* s) {
    // Clear the blocks to start with a clean state
    memset(s->x, 0, sizeof(s->x[0]) * BLOCK_NUMBER);

    // Iterate over each of the 64 bundles
    for(int bit = 0; bit < 64; bit++) {
        // Iterate over each block within the state structure
        for(int block_index = 0; block_index < BLOCK_NUMBER; block_index++) {
            // Extract the bit for the current block from the bundle
            uint64_t bitValue = (bundles[bit] >> (BLOCK_NUMBER - 1 - block_index)) & 0x01;

            // Place the extracted bit in the correct position in the 64-bit block
            s->x[block_index] |= bitValue << (63 - bit);
        }
    }
}