#ifndef DIFFUSION_LAYER_ROUND_H
#define DIFFUSION_LAYER_ROUND_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include "constants.h"
#include "ladybug.h"
#include "api.h"

#define BLOCK_SIZE 8
#define MERSENNE_EXPONENT 7
#define Mp ((1 << MERSENNE_EXPONENT) - 1) // Modulus for encryption operation
static const uint8_t invN = (1 << MERSENNE_EXPONENT) / BLOCK_SIZE;

static const uint8_t NMNT[BLOCK_SIZE][BLOCK_SIZE] = {{1, 1, 1, 1, 1, 1, 1, 1},
                                                     {1, 111, 1, 0, 126, 16, 126, 0},
                                                     {1, 1, 126, 126, 1, 1, 126, 126},
                                                     {1, 0, 126, 111, 126, 0, 1, 16},
                                                     {1, 126, 1, 126, 1, 126, 1, 126},
                                                     {1, 16, 1, 0, 126, 111, 126, 0},
                                                     {1, 126, 126, 1, 1, 126, 126, 1},
                                                     {1, 0, 126, 16, 126, 0, 1, 111}};

static const uint8_t ONMNT[BLOCK_SIZE][BLOCK_SIZE] = {{1, 1, 1, 1, 1, 1, 1, 1},
                                                      {82, 82, 3, 124, 45, 45, 124, 3},
                                                      {111, 0, 16, 0, 111, 0, 16, 0},
                                                      {82, 45, 3, 3, 45, 82, 124, 124},
                                                      {1, 126, 1, 126, 1, 126, 1, 126},
                                                      {3, 3, 45, 82, 124, 124, 82, 45},
                                                      {0, 111, 0 ,16, 0, 111, 0, 16},
                                                      {124, 3, 82, 82, 3, 124, 45, 45}};

static const uint8_t O2NMNT[BLOCK_SIZE][BLOCK_SIZE] = {{72, 19, 19, 72, 5, 99, 28, 122},
                                                      {19, 5, 122, 108, 28, 72, 72, 28},
                                                      {19, 122, 122, 19, 28, 55, 72, 99},
                                                      {72, 108, 19, 55, 5, 28, 28, 5},
                                                      {5, 28, 28, 5, 55, 19, 108, 72},
                                                      {99, 72, 55, 28, 19, 122, 122, 19},
                                                      {28, 72, 72, 28, 108, 122, 5, 19},
                                                      {122, 28, 99, 5, 72, 19, 19, 72}};

void forward_transform_round_function(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]);
void forward_transform(ladybug_state_t *state, const uint8_t transform_matrix[BLOCK_SIZE][BLOCK_SIZE]);
void split_state_into_data_bytes(ladybug_state_t *state, uint8_t *data_byte, int block_index);
void combine_data_bytes_to_state(const uint8_t *data_byte, ladybug_state_t *state, int block_index);
void print_data_byte(ladybug_state_t *state);
#endif //DIFFUSION_LAYER_ROUND_H
