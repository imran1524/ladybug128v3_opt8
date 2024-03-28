#ifndef DIFFUSION_LAYER_ENCRYPT_H
#define DIFFUSION_LAYER_ENCRYPT_H

#include <stdint.h>
#include "constants.h"
#include "word.h"

typedef struct {
    uint64_t x[BLOCK_NUMBER];
}state_t;

int crypto_aead_encrypt(
        unsigned char *c, unsigned long long *clen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *ad, unsigned long long adlen,
        const unsigned char *nsec,
        const unsigned char *npub,
        const unsigned char *k
);

int crypto_aead_decrypt(
        unsigned char *m, unsigned long long *mlen,
        unsigned char *msec,
        const unsigned char *c, unsigned long long clen,
        const unsigned char *ad, unsigned long long adlen,
        const unsigned char *npub,
        const unsigned char *k
        );

#endif //DIFFUSION_LAYER_ENCRYPT_H
