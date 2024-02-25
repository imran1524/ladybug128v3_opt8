#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"
#include "constants.h"



void print_vector(uint8_t *vector, int vector_size){
    for(uint8_t i = 0; i < vector_size; i++){
        printf("vector[%d] = %u\n", i, vector[i]);
    }
}

void print_state(data_struct *state){
    for(int i = 0; i < BLOCK_NUMBER; i++){
        printf("state.x[%d] = %llx\n", i, state->x[i]);
    }
}

void print_bitstring(uint8_t vector, uint8_t bit_number) {
    for(int8_t bit_index = bit_number - 1; bit_index >= 0; bit_index--) {
        printf("%d", (vector >> bit_index) & 0x01);
    }
}

// Function to convert text to 64-bit blocks and store in data_struct
void text_to_64bit_blocks_data_struct(const char* text, data_struct* s) {
    size_t maxTextLength = block_number * 8; // Max text length to fit in 5 x 64-bit blocks
    size_t textLength = strlen(text);

    if (textLength > maxTextLength) {
        textLength = maxTextLength; // Truncate text to fit in 320 bits
    }

    // Initialize blocks to 0
    memset(s->x, 0, block_number * sizeof(uint64_t));

    // Pack characters into 64-bit blocks
    for (size_t i = 0; i < textLength; ++i) {
        size_t blockIndex = i / 8; // Determine which block the character belongs to
        size_t charIndex = i % 8; // Determine the position within the block
        ((uint8_t*)(&s->x[blockIndex]))[charIndex] = (uint8_t)text[i]; // Pack character
    }
}

// Function to combine the data from data_struct and get the text back
void blocks_to_text(data_struct* s, char* outText, size_t maxTextLength) {
    size_t byteIndex = 0;
    for (size_t i = 0; i < block_number; ++i) {
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
