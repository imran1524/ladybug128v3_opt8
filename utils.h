#ifndef DIFFUSION_LAYER_UTILS_H
#define DIFFUSION_LAYER_UTILS_H

#include "round.h"
#include "aead.h"

void print_vector(uint8_t *vector, size_t vector_size);
void print_state(state_t *state);
void print_bitstring(uint8_t vector, uint8_t bit_number);
void text_to_64bit_blocks_data_struct(const char* text, state_t* s);
void blocks_to_text(state_t* s, char* outText, size_t maxTextLength);
void print_bundles(uint8_t *bundle, uint8_t size);
void create_blocks_from_bundles(uint8_t bundles[64], state_t *s);
void generate_nonce(uint8_t *nonce, size_t nonce_len);
#endif //DIFFUSION_LAYER_UTILS_H
