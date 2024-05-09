#ifndef LADYBUG_ENCRYPT_H
#define LADYBUG_ENCRYPT_H

#include <stdint.h>
#include "constants.h"
#include "word.h"

typedef struct
{
    uint64_t x[5];
    uint32_t w[5][2];
    uint16_t h[5][4];
    uint8_t b[5][8];
} ladybug_state_t;

#ifdef LADYBUG_AEAD_RATE
#define LADYBUG_KEYWORDS ((CRYPTO_KEYBYTES + 7) / 8)

typedef union
{
    uint64_t x[LADYBUG_KEYWORDS];
    uint32_t w[LADYBUG_KEYWORDS][2];
    uint8_t b[LADYBUG_KEYWORDS][8];
} ladybug_key_t;

#if !LADYBUG_INLINE_MODE
void loadkey(ladybug_key_t *key, const uint8_t *k);
void ladybug_initaead(ladybug_state_t *s, const ladybug_key_t *key,
                    const uint8_t *npub);
void ladybug_adata(ladybug_state_t *s, const uint8_t *ad, uint64_t adlen);
void ladybug_encrypt(ladybug_state_t *s, uint8_t *c, const uint8_t *m,
                   uint64_t mlen);
void ladybug_decrypt(ladybug_state_t *s, uint8_t *m, const uint8_t *c,
                   uint64_t clen);
void ladybug_final(ladybug_state_t *s, const ladybug_key_t *k);

#endif // LADYBUG_INLINE_MODE

#endif // LADYBUG_AEAD_RATE
#endif // LADYBUG_LAYER_ENCRYPT_H
