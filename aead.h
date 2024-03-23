#ifndef DIFFUSION_LAYER_AEAD_H
#define DIFFUSION_LAYER_AEAD_H

#include <stdint.h>
#include "constants.h"
#include "word.h"

typedef struct {
    uint64_t x[BLOCK_NUMBER];
}state_t;
 size_t crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                           const unsigned char* m, size_t mlen,
                           const unsigned char* ad, size_t adlen,
                           const unsigned char* nsec,
                           const unsigned char*npub,
                           const unsigned char* k);

size_t crypto_aead_decrypt(unsigned char* m, unsigned long long *mlen,
                           const unsigned char* c, size_t clen,
                           const unsigned char* ad, size_t adlen,
                           const unsigned char* nsec,
                           const unsigned char* npub,
                           const unsigned char* k);

#endif //DIFFUSION_LAYER_AEAD_H
