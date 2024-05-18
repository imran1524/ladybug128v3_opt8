#include "api.h"
#include "ladybug.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "utils.h"
#include "printstate.h"
#include <stdio.h>

#if !LADYBUG_INLINE_MODE
#undef forceinline
#define forceinline
#endif

#ifdef LADYBUG_AEAD_RATE

// forceinline void ladybug_state_init(ladybug_state_t* state) {
//     // printf("I'm inside ladybug_state_init\n");
//     memset(state -> x, 0, sizeof(state -> x));
// }

//Loading the key to first two blocks of the state
forceinline void ladybug_loadkey(ladybug_key_t* key, const uint8_t* k) {
    #if CRYPTO_KEYBYTES == 16
        printf("I'm inside ladybug_loadkey\n");
        INSERT(key->b[0], k, 8);
        INSERT(key->b[1], k + 8, 8);
    #endif
}

//Force inline function to initialize the AEAD
// Force the function to be inlined
forceinline void ladybug_initaead(ladybug_state_t* s, const ladybug_key_t* key, const uint8_t* npub) {
    #if CRYPTO_KEYBYTES == 16
        if (LADYBUG_AEAD_RATE == 8) {
            // printf("I'm inside ladybug_initaead\n");
            //Inserting the IV to the state
            s->x[0] = LADYBUG_128_IV;
        }
        //Inserting the key to the state
        memcpy(s->b[1], key->b[0], 16);
    #endif
        //Inserting the nonce to the state
        INSERT(s->b[3], npub, 8);
        INSERT(s->b[4], npub + 8, 8);

    // printf("Print state before permutation:\n");
    // print_data_byte(s);
        P1(s);
    // printf("Print state after permutation:\n");
    // print_data_byte(s);

    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[3], key->b[0], 16);
    #endif
}

//Force inline function to process the associated data
forceinline void ladybug_adata(ladybug_state_t* s, const uint8_t* ad, uint64_t adlen) {
    // const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
    if (adlen) {
        //Full associated data blocks
        // printf("I'm inside ladybug_adata\n");
        while (adlen >= LADYBUG_AEAD_RATE) {
            ABSORB(s->b[0], ad, 8);
            if (LADYBUG_AEAD_RATE == 8) ABSORB(s->b[1], ad + 8, 8);
            // printf("Print state before permutation:\n");
            // print_data_byte(s);
            P1(s);
            // printf("Print state after permutation:\n");
            // print_data_byte(s);
            ad += LADYBUG_AEAD_RATE;
            adlen -= LADYBUG_AEAD_RATE;
        }

        //Final associated data block
        int i = 0;
        if (LADYBUG_AEAD_RATE == 8 && adlen >= 8) {
            ABSORB(s->b[0], ad, 8);
            i = 1;
            ad += 8;
            adlen -= 8;
        }

        ABSORB(s->b[i], ad, adlen);
        s->b[i][7 - adlen] ^= 0x80;
        P1(s);
    }

    // printf("State before domain separation\n");
    // print_data_byte(s);
    s->b[4][0] ^= 1;
    // printstate("domain separation", s);
    // printf("State after domain separation\n");
    // print_data_byte(s);
}

//Force inline function to encrypt the plaintext
forceinline void ladybug_encrypt(ladybug_state_t* s, uint8_t* c, const uint8_t* m, uint64_t mlen) {
    printf("I'm inside ladybug_encrypt\n");
    // printf("mlen = %d\n", mlen);
    // printf("State before encryption:\n");
    // print_data_byte(s);

    //Full plaintext blocks
    while (mlen >= LADYBUG_AEAD_RATE) {
        ENCRYPT(s->b[0], c, m, 8);
        if (LADYBUG_AEAD_RATE == 8) ENCRYPT(s->b[1], c + 8, m + 8, 8);
        // printf("State before permutation:\n");
        // print_state(s);
        P1(s);
        // printf("State after encryotion block\n");
        // print_data_byte(s);
        m += LADYBUG_AEAD_RATE;
        c += LADYBUG_AEAD_RATE;
        mlen -= LADYBUG_AEAD_RATE;
    }

    //Final plaintext block
    int i = 0;
    if (LADYBUG_AEAD_RATE == 8 && mlen >= 8) {
        printf("I'm inside final plaintext text\n");
        ENCRYPT(s->b[0], c, m, 8);
        i = 1;
        m += 8;
        c += 8;
        mlen -= 8;
    }

    printf("Enryption: Processing final plaintext block\n");
    ENCRYPT(s->b[i], c, m, mlen);
    s->b[i][7 - mlen] ^= 0x80;
    // printf("Encrypted padding final state:\n");
    // print_data_byte(s);
}

forceinline void ladybug_decrypt(ladybug_state_t* s, uint8_t* m, const uint8_t* c, uint64_t clen) {
  // const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
  /* full ciphertext blocks */
  while (clen >= LADYBUG_AEAD_RATE) {
    printf("Decryption: Processing full ciphertext block\n");
    DECRYPT(s->b[0], m, c, 8);
    if (LADYBUG_AEAD_RATE == 8) DECRYPT(s->b[1], m + 8, c + 8, 8);
    printf("Inset ciphertext state:\n");
    // P(s, nr);
    P1(s);
    m += LADYBUG_AEAD_RATE;
    c += LADYBUG_AEAD_RATE;
    clen -= LADYBUG_AEAD_RATE;
  }

  /* final ciphertext block */
  printf("Decryption: Processing final ciphertext block\n");
  int i = 0;
  if (LADYBUG_AEAD_RATE == 8 && clen >= 8) {
    DECRYPT(s->b[0], m, c, 8);
    i = 1;
    m += 8;
    c += 8;
    clen -= 8;
  }

    printf("Decryption: Processing padded block\n");
    DECRYPT(s->b[i], m, c, clen);
    s->b[i][7 - clen] ^= 0x80;
    // printf("Decrypted padding final state:\n");
    // print_data_byte(s);
}

forceinline void ladybug_final(ladybug_state_t* s, const ladybug_key_t* key) {
    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[LADYBUG_AEAD_RATE / 8], key->b[0], 16);
    #endif
    // printf("I'm inside ladybug_final\n");
    // printf("Final 1st key xor\n");
    // print_data_byte(s);
    // printstate("final 1st key xor", s);
    P1(s);

    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[3], key->b[0], 16);
    #endif
    // printstate("final 2nd key xor", s);
    // printf("Final 2nd key xor\n");
    // print_data_byte(s);
}

//ENCRYPTION AEAD
int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub, 
                        const unsigned char* k) {
    ladybug_state_t s;
    (void)nsec;
    *clen = mlen + CRYPTO_ABYTES;

    // ladybug_state_init(&s);
    // // printf("Encryption: State initialized:\n");
    // print_data_byte(&s);

    ladybug_key_t key;
    ladybug_loadkey(&key, k);
    // printf("Encryption: Key loaded:\n");
    // print_data_byte(&key);

    ladybug_initaead(&s, &key, npub);
    // printf("Encryption: State after initialization:\n");
    // print_data_byte(&s);

    ladybug_adata(&s, ad, adlen);
    // printf("Encryption: State after processing associated data:\n");
    // print_data_byte(&s);

    ladybug_encrypt(&s, c, m, mlen);
    // printf("Encryption: State after encryption:\n");
    // print_data_byte(&s);

    ladybug_final(&s, &key);
    // printf("Encryption: State after finalization:\n");
    // print_data_byte(&s);

    //Generate the tag
    SQUEEZE(c + mlen, s.b[3], 8);
    SQUEEZE(c + mlen + 8, s.b[4], 8);
    return 0;
}

//DECRYPTION AEAD
int crypto_aead_decrypt(unsigned char* m, unsigned long long* mlen, 
                        unsigned char* nsec, 
                        const unsigned char* c, unsigned long long clen, 
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* npub, const unsigned char* k) {
    ladybug_state_t s;
    (void)nsec;

    if (clen < CRYPTO_ABYTES) {
        return -1;
    }

    *mlen = clen - CRYPTO_ABYTES;
    // printf("Decryption: Ciphertext:\n");
    // print_vector(c, clen);

    // printf("Decryption: Associated date:\n");
    // print_vector(ad, adlen);

    // ladybug_state_init(&s);
    // printf("Decryption: State initialized:\n");
    // print_data_byte(&s);

    ladybug_key_t key;
    ladybug_loadkey(&key, k);
    // printf("Decryption: Key loadecld:\n");
    // print_data_byte(&key);

    ladybug_initaead(&s, &key, npub);
    // printf("Decryption: State after initialization:\n");
    // print_data_byte(&s);

    ladybug_adata(&s, ad, adlen);
    // printf("Decryption: State after processing associated data:\n");
    // print_data_byte(&s);

    ladybug_decrypt(&s, m, c, clen);
    // printf("Decryption: State after decryption:\n");
    // print_data_byte(&s);

    ladybug_final(&s, &key);
    // printf("Decryption: State after finalization:\n");
    // print_data_byte(&s);
// 
    uint8_t r = 0;
    r |= VERIFY(s.b[3], c + clen, 8);
    r |= VERIFY(s.b[4], c + clen + 8, 8);
    return ((((int)r - 1) >> 8) & 1) - 1;
}

#endif // LADYBUG_AEAD_RATE`