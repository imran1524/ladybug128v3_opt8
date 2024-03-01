#ifndef DIFFUSION_LAYER_PERMUTATIONS_H
#define DIFFUSION_LAYER_PERMUTATIONS_H

#include "utils.h"
#include "constants.h"
#include "round.h"

void FP3 (data_struct *s);
void IP3 (data_struct *s);

void forward_permutation(data_struct *s);
void inverse_permutation(data_struct *s);

extern uint8_t forward_s_box[1 << s_box_size];
extern uint8_t inverse_s_box[1 << s_box_size];

#endif //DIFFUSION_LAYER_PERMUTATIONS_H
