#ifndef DIFFUSION_LAYER_AEAD_H
#define DIFFUSION_LAYER_AEAD_H

#include <stdint.h>
#include "constants.h"

typedef struct {
    uint64_t x[BLOCK_NUMBER];
}state_t;
#endif //DIFFUSION_LAYER_AEAD_H
