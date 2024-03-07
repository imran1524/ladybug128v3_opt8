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

    //XORing key with state after initialing layer
    s.x[3] ^= K0;
    s.x[4] ^= K1;

    printf("State after inilialization WITH KEY\n");
    print_state(&s);

    //END OF INITIALIZATIO

    //START OF ASSOCIATED DATA DOMAIN

    //START OF PLAINTEXT DOMAIN
    printf("Associated data\n");
    if (adlen){
        size_t block = 0;
        while(adlen >= RATE){
            printf("Block = %d\n", block);
            s.x[0] ^= LOAD_BYTES(ad, 8);
            FP1(&s);
            ad += RATE;
            adlen -= RATE;
            block++;
        }
        //FINAL ASSOCIATED DATA BLOCK
        s.x[0] ^= LOAD_BYTES(ad, adlen);
        s.x[0] ^= PAD(adlen);
        print_state(&s);
        FP1(&s);
    }
    //DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND PLAINTEXT
    s.x[4] ^= 1;
    printf("State after domain separation\n");
    print_state(&s);

    //END OF ASSOCIATED DOMAIN

    //START OF PLAINTEXT DOMAIN
    while (mlen >= RATE){
        //LOADING 8 BYTES FROM THE MESSAGE AND XOR WITH THE FIRST BLOCK OF THE STATE
        LOAD_BYTES(m, 8);
        printf("Message block:\n");
        printf("\n");
        print_vector(m, 8);
        s.x[0] ^= LOAD_BYTES(m, 8);
        printf("\n");
        STORE_BYTES(c, s.x[0], 8);

        printf("Ciphertext block:\n");
        print_vector(c, 8);

        printf("\n");
        printf("absorb plaintext\n");
        print_state(&s);

        m += RATE;
        c += RATE;
        mlen -= RATE;
    }

    //FINAL PLAINTEXT BLOCK
    s.x[0] ^= LOAD_BYTES(m, mlen);
    STORE_BYTES(c, s.x[0], mlen);
    s.x[0] ^= PAD(mlen);
    c += mlen;
    printf("\n");
    printf("Padded plaintext\n");
    print_state(&s);
    printf("\n");
    //END OF PLAINtext

    //FINALIZATION DOMAIN
    //START OF FINALIZATION
    s.x[1] ^= K0;
    s.x[2] ^= K1;
    printf("STATE AFTER FIRST KEY XOR\n");
    print_state(&s);

    printf("\n");
    //APPLY FORWARD PERMUTATION
    FP1(&s);
    s.x[3] ^= K0;
    s.x[4] ^= K1;
    printf("STATE AFTER second KEY XOR\n");
    print_state(&s);


    //SET TAG
    STORE_BYTES(c, s.x[3], 8);
    STORE_BYTES(c + 8, s.x[4], 8);

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
