#ifndef DIFFUSION_LAYER_UTILS_H
#define DIFFUSION_LAYER_UTILS_H

#include "round.h"
#include "ladybug.h"
#include "cJSON/cJSON.h"

void print_vector(uint8_t *vector, size_t vector_size);
void print_state(ladybug_state_t *state);
void print_bitstring(uint8_t vector, uint8_t bit_number);
void text_to_64bit_blocks_data_struct(const char* text, ladybug_state_t* s);
void blocks_to_text(ladybug_state_t* s, char* outText, size_t maxTextLength);
void print_bundles(uint8_t *bundle, uint8_t size);
void create_blocks_from_bundles(uint8_t bundles[64], ladybug_state_t *s);
void generate_nonce(uint8_t *nonce, size_t nonce_len);
void print_character(uint8_t *vector, size_t vector_size);
char* parseJsonFile(char *filename, long *outFileSize);
void printTestCase(cJSON *test);
void hex_string_to_binary(const char* hex_str, unsigned char* binary, size_t binary_len);
void print_binary(unsigned char* binary, size_t binary_len);
void print_Hexvalue(uint8_t *vector, size_t vector_size);
#endif //DIFFUSION_LAYER_UTILS_H
