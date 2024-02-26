#ifndef DIFFUSION_LAYER_UTILS_H
#define DIFFUSION_LAYER_UTILS_H

#include "round.h"

void print_vector(uint8_t *vector, int vector_size);
void print_state(data_struct *state);
void print_bitstring(uint8_t vector, uint8_t bit_number);
void text_to_64bit_blocks_data_struct(const char* text, data_struct* s);
void blocks_to_text(data_struct* s, char* outText, size_t maxTextLength);
void print_bundles(uint8_t *bundle, uint8_t size);
void create_blocks_from_bundles(uint8_t bundles[64], data_struct *s);
#endif //DIFFUSION_LAYER_UTILS_H
