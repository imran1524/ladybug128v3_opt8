#ifndef DIFFUSION_LAYER_CONSTANTS_H
#define DIFFUSION_LAYER_CONSTANTS_H

#include <stdint.h>

#define LADYBUG_128_KEYBYTES 16

#define BLOCK_NUMBER 5
#define SBOX_SIZE 5

#define TRANSFORM_P 7
#define TRANSFORM_N 8
#define AEAD_KEY_BYTES 16
#define AEAD_RATE_BYTES 8
#define PRF_IN_RATE 32



#define ROUNDS 1

#define AEAD_128_IV \
(((uint64_t)(AEAD_KEY_BYTES * 8) << 56)    |\
 ((uint64_t)(AEAD_RATE_BYTES * 8) << 48)   |\
 ((uint64_t)(TRANSFORM_P) << 40)           |\
 ((uint64_t)(TRANSFORM_N) << 32))
#endif //DIFFUSION_LAYER_CONSTANTS_H
