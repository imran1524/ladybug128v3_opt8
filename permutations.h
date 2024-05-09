#ifndef DIFFUSION_LAYER_PERMUTATIONS_H
#define DIFFUSION_LAYER_PERMUTATIONS_H

#include "utils.h"
#include "constants.h"
#include "round.h"
#include "ladybug.h"
#include "printstate.h"
#include "config.h"

void P1 (ladybug_state_t *s);

void forward_permutation(ladybug_state_t *s);

extern uint8_t forward_s_box[1 << SBOX_SIZE];

#endif //DIFFUSION_LAYER_PERMUTATIONS_H
