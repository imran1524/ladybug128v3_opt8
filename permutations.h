#ifndef DIFFUSION_LAYER_PERMUTATIONS_H
#define DIFFUSION_LAYER_PERMUTATIONS_H

#include "utils.h"
#include "constants.h"
#include "round.h"
#include "encrypt.h"

void FP1 (state_t *s);
void IP1 (state_t *s);

void forward_permutation(state_t *s);
void inverse_permutation(state_t *s);

extern uint8_t forward_s_box[1 << s_box_size];
extern uint8_t inverse_s_box[1 << s_box_size];

#endif //DIFFUSION_LAYER_PERMUTATIONS_H
