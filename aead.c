#include "api.h"
#include "aead.h"
#include "permutations.h"
#include "utils.h"

//ENCRYPTION
size_t crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                           const unsigned char* m, unsigned long long mlen,
                           const unsigned char* ad, unsigned long long adlen,
                           const unsigned char* nsec,
                           const unsigned char*npub,
                           const unsigned char* k){
    (void)nsec;
    //SET THE POINTER OF CIPHERTEXT BY ADDING MESSAGE LENGTH AND ASSOCIATED DATA LENGTH
    *clen = mlen + AD_BYTES;

    //LOAD KEY AND NONCE
    //key, k is 128-bits that is divided into two 64-bit keys, public nonce, npub is 128-bits
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

    //INITIALIZATION DOMAIN
    state_t s;
    s.x[0] = AEAD_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;

    printf("State before inilialization\n");
    print_state(&s);
    printf("State after inilialization\n");
    FP1(&s);
    printf("\n");


    //ASSOCIATED DATA DOMAIN



    //PLAINTEXT DOMAIN



    //FINALIZATION DOMAIN

    return 0;
}




//DECRYPTION
size_t crypto_aead_decryption(){


    size_t i = 0;
    size_t result = 0;
    return result;
}
//INITIALIZATION DOMAIN



//ASSOCIATED DATA DOMAIN



//CIPHERTEXT DOMAIN



//FINALIZATION
