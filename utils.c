#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "utils.h"
#include "constants.h"
#include "api.h"

//void generate_nonce(uint8_t *nonce, size_t nonce_len) {
//    arc4random_buf(nonce, nonce_len);
//}

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <Wincrypt.h>
#else
#include <stdio.h>
#endif

void generate_nonce(uint8_t *nonce, size_t nonce_len) {
    if (nonce == NULL) return;

#if defined(_WIN32) || defined(_WIN64)
    HCRYPTPROV prov = 0;
    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
        exit(EXIT_FAILURE); // Consider a more graceful error handling strategy
    }
    if (!CryptGenRandom(prov, (DWORD)nonce_len, nonce)) {
        CryptReleaseContext(prov, 0);
        exit(EXIT_FAILURE); // Consider a more graceful error handling strategy
    }
    CryptReleaseContext(prov, 0);
#else
    FILE *file = fopen("/dev/urandom", "rb");
    if (!file) {
        exit(EXIT_FAILURE); // Consider a more graceful error handling strategy
    }
    fread(nonce, 1, nonce_len, file);
    fclose(file);
#endif
}

void print_vector(uint8_t *vector, size_t vector_size){
    for(uint8_t i = 0; i < vector_size; i++){
        printf("block[%d] = %d\n", i, vector[i]);
    }
}

void print_Hexvalue(uint8_t *vector, size_t vector_size){
    for(uint8_t i = 0; i < vector_size; i++){
        printf("%0x", vector[i]);
    }
}

void print_bundles(uint8_t *bundle, uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        printf("bundle[%d] = %d\n", i, bundle[i]);
    }
}

void print_state(ladybug_state_t *state) {
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
void text_to_64bit_blocks_data_struct(const char* text, ladybug_state_t* s) {
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
void blocks_to_text(ladybug_state_t* s, char* outText, size_t maxTextLength) {
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

void create_blocks_from_bundles(uint8_t bundles[64], ladybug_state_t* s) {
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

char* parseJsonFile(char *filename, long *outFileSize) {
    FILE *jsonFile = fopen(filename, "r");
    if (jsonFile == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(jsonFile, 0, SEEK_END);
    long fileSize = ftell(jsonFile);
    rewind(jsonFile);

    char *jsonString = (char *)malloc((fileSize + 1) * sizeof(char));
    if(jsonString == NULL) {
        printf("Failed to allocate memory\n");
        fclose(jsonFile);
        return NULL;
    }

    fread(jsonString, 1, fileSize, jsonFile);
    jsonString[fileSize] = '\0';
    fclose(jsonFile);

    *outFileSize = fileSize;
    return jsonString;
}

void hex_string_to_binary(const char* hex_str, unsigned char* binary, size_t binary_len) {
    size_t hex_len = strlen(hex_str);
    size_t i, j;

    if (hex_len % 2 != 0 || hex_len / 2 > binary_len) {
        // Invalid hex string or insufficient binary buffer length
        return;
    }

    for (i = 0, j = 0; i < hex_len; i += 2, j++) {
        char byte[3] = {hex_str[i], hex_str[i + 1], '\0'};
        binary[j] = (unsigned char)strtoul(byte, NULL, 16);
    }
}