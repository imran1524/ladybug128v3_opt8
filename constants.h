#ifndef LADYBUG_CONSTANTS_H
#define LADYBUG_CONSTANTS_H

#include <stdint.h>

#define LADYBUG_128_KEYBYTES 16
#define BLOCK_NUMBER 5
#define SBOX_SIZE 5
#define TRANSFORM_P 7
#define TRANSFORM_N 8
#define LADYBUG_128_RATE_BYTES 8
#define ROUNDS 1

#define LADYBUG_128_IV \
(((uint64_t)(LADYBUG_128_KEYBYTES * 8) << 56)    |\
 ((uint64_t)(LADYBUG_128_RATE_BYTES * 8) << 48)   |\
 ((uint64_t)(TRANSFORM_P) << 40)           |\
 ((uint64_t)(TRANSFORM_N) << 32))
#endif //LADYBUG_CONSTANTS_H
