#include "crypto_aead.h"
#include "api.h"
#include "ladybug.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "utils.h"

//ENCRYPTION
int crypto_aead_encrypt(
        unsigned char *c, unsigned long long *clen,
        const unsigned char *m, unsigned long long mlen,
        const unsigned char *ad, unsigned long long adlen,
        const unsigned char *nsec, const unsigned char *npub,
        const unsigned char *k
){
    (void)nsec;
    //SET THE POINTER OF CIPHERTEXT BY ADDING MESSAGE LENGTH AND ASSOCIATED DATA LENGTH
    *clen = mlen + CRYPTO_ABYTES;

    //LOAD KEY AND NONCE
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

    //INITIALIZATION:
    state_t s;
    s.x[0] = LADYBUG_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;

    //APPLY FORWARD PERMUTATION LAYER
    FP1(&s);

    //XORING KEY 0*||K
    s.x[3] ^= K0;
    s.x[4] ^= K1;

//PROCESSING ASSOCIATED DATA
    if (adlen){
        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= LADYBUG_128_RATE_BYTES){
//            printf("adlen in encryption = %zu\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);

            //APPLY A FORWARD PERMUTATION OPERATION
            FP1(&s);


//            printf("adlen_final = %d\n", adlen);
            ad += LADYBUG_128_RATE_BYTES;
            adlen -= LADYBUG_128_RATE_BYTES;
        }

        //FINAL ASSOCIATED DATA BLOCK
        s.x[0] ^= LOAD_BYTES(ad, adlen);
        s.x[0] ^= PAD(adlen);

        //APPLY FORWARD PERMUTATION
        FP1(&s);
    }

    //DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND PLAINTEXT
    s.x[4] ^= 1;
    size_t block = 0;
    while (mlen >= LADYBUG_AEAD_RATE){
        s.x[0] ^= LOAD_BYTES(m, 8);
        STORE_BYTES(c, s.x[0], 8);

        //APPLY FORWARD PERMUTATION
        FP1(&s);

        m += LADYBUG_128_RATE_BYTES;
        c += LADYBUG_128_RATE_BYTES;
        mlen -= LADYBUG_128_RATE_BYTES;
        block++;
    }

    //LOADING FINAL MESSAGE BLOCK AND XOR WITH THE STATE
    s.x[0] ^= LOAD_BYTES(m, mlen); 
    STORE_BYTES(c, s.x[0], mlen);
    s.x[0] ^= PAD(mlen);
    c += mlen;

    //FINALISE
    s.x[1] ^= K0;
    s.x[2] ^= K1;

    //APPLY FORWARD PERMUTATION
    FP1(&s);

    //XORING K||0*
    s.x[3] ^= K0;
    s.x[4] ^= K1;

    //SET TAG
    STORE_BYTES(c, s.x[3], 8); 
    STORE_BYTES(c + 8, s.x[4], 8);
    return 0;
}

//DECRYPTION AEAD

int crypto_aead_decrypt(
            unsigned char *m, unsigned long long *mlen,
            unsigned char *nsec,
            const unsigned char *c, unsigned long long clen,
            const unsigned char *ad, unsigned long long adlen,
            const unsigned char *npub,
            const unsigned char *k
    ){

    (void)nsec;

    if(clen < CRYPTO_ABYTES){
        return -1;
    }

    //SET PLAINTEXT SIZE
    *mlen = clen - CRYPTO_ABYTES;

    //LOAD KEY AND NONCE
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

//START OF INITIALIZATION DOMAIN
    state_t s;
    s.x[0] =LADYBUG_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;

    //APPLY FORWARD PERMUTATION
    FP1(&s);

    s.x[3] ^= K0;
    s.x[4] ^= K1;

//START OF ASSOCIATED DATA DOMAIN
    if(adlen){
        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= LADYBUG_128_RATE_BYTES){
//            printf("adlen in decryption = %d\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);

            //APPLY FORWARD PERMUTATION
            FP1(&s);

            ad += LADYBUG_128_RATE_BYTES;
            adlen -= LADYBUG_128_RATE_BYTES;
        }

        //FINAL ASSOCIATED DATA BLOCK
        s.x[0] ^= LOAD_BYTES(ad, adlen);
        s.x[0] ^= PAD(adlen);

        //APPLY FORWARD PERMUTATION
        FP1(&s);
    }

//DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND CIPHERTEXT
    s.x[4] ^= 1;

//START OF CIPHERTEXT DOMAIN
    clen -= CRYPTO_ABYTES;
    size_t block = 0;
    while(clen >= LADYBUG_128_RATE_BYTES){
        uint64_t c0 = LOAD_BYTES(c, 8);
        STORE_BYTES(m, s.x[0] ^ c0, 8);
        s.x[0] = c0;

        //INVERSE PERMUTATION
        FP1(&s);
        m += LADYBUG_128_RATE_BYTES;
        c += LADYBUG_128_RATE_BYTES;
        clen -= LADYBUG_128_RATE_BYTES;
        block++;
    }

    //FINAL CIPHERTEXT BLOCK
//    printf("clen = %zu\n", clen);
    uint64_t c0 = LOAD_BYTES(c, clen);
    STORE_BYTES(m, s.x[0] ^c0, clen);
    s.x[0] = CLEAR_BYTES(s.x[0], clen);
    s.x[0] |= c0;
    s.x[0] ^= PAD(clen);
    c += clen;

    //FINALIZE
    s.x[1] ^= K0;
    s.x[2] ^= K1;

    //APPLY INVERSE PERMUTATION
    FP1(&s);
    s.x[3] ^= K0;
    s.x[4] ^= K1;

    //SET TAG
    uint8_t t[16];
    STORE_BYTES(t, s.x[3], 8); 
    STORE_BYTES(t + 8, s.x[4], 8); 
    
    //VERIFY TAG BY COMPARING THE GENERATED TAG WITH THE ORIGINAL TAG
    size_t i;
    size_t result = 0;
    for(i = 0; i < CRYPTO_ABYTES; i++){
        result |= c[i] ^ t[i];
//        printf("Result = %zu\n", result);
    }

    //VERIFY TAG (THIS OPERATION MUST BE CONSTANT TIME)
    result = (((result - 1) >> 8) & 1) - 1;
    return  result;
}

void print_binary(unsigned char* binary, size_t binary_len) {
    for(size_t i = 0; i < binary_len; i++) {
        // Print each byte in binary
        for(int bit = 7; bit >= 0; bit--) {
            putchar((binary[i] >> bit) & 1 ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}
