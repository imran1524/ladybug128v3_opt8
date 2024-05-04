#ifndef DIFFUSION_LAYER_PERMUTATIONS_H
#define DIFFUSION_LAYER_PERMUTATIONS_H

#include "utils.h"
#include "constants.h"
#include "round.h"
#include "ladybug.h"
#include "printstate.h"
#include "config.h"

void FP1 (ladybug_state_t *s);
void IP1 (ladybug_state_t *s);

void forward_permutation(ladybug_state_t *s);
void inverse_permutation(ladybug_state_t *s);

extern uint8_t forward_s_box[1 << SBOX_SIZE];
extern uint8_t inverse_s_box[1 << SBOX_SIZE];

#endif //DIFFUSION_LAYER_PERMUTATIONS_H
