#include "api.h"
#include "ladybug.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "utils.h"
#include "printstate.h"
#include <stdio.h>
#include <string.h>

#if !LADYBUG_INLINE_MODE
#undef forceinline
#define forceinline
#endif

#ifdef LADYBUG_AEAD_RATE

// Load the key into the state
forceinline void ladybug_loadkey(ladybug_key_t* key, const uint8_t* k) {
    #if CRYPTO_KEYBYTES == 16
        INSERT(key->b[0], k, 8);
        INSERT(key->b[1], k + 8, 8);
    #endif
}

// Explicitly initialize the state during AEAD initialization
forceinline void ladybug_initaead(ladybug_state_t* s, const ladybug_key_t* key, const uint8_t* npub) {
    // Initialize specific components based on the key size and AEAD rate
    #if CRYPTO_KEYBYTES == 16
        s->x[0] = (LADYBUG_AEAD_RATE == 8) ? LADYBUG_128_IV : 0;
        memcpy(s->b[1], key->b[0], 16);
    #endif
    
    // Insert nonce
    INSERT(s->b[3], npub, 8);
    INSERT(s->b[4], npub + 8, 8);

    // Apply permutation
    P1(s);

    // XOR key into state
    memxor(s->b[3], key->b[0], 16);
}

// Process associated data
forceinline void ladybug_adata(ladybug_state_t* s, const uint8_t* ad, uint64_t adlen) {
    const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
    if (adlen) {
        while (adlen >= LADYBUG_AEAD_RATE) {
            ABSORB(s->b[0], ad, 8);
            if (LADYBUG_AEAD_RATE == 16) ABSORB(s->b[1], ad + 8, 8);
            P1(s);
            ad += LADYBUG_AEAD_RATE;
            adlen -= LADYBUG_AEAD_RATE;
        }
        int i = 0;
        if (LADYBUG_AEAD_RATE == 16 && adlen >= 8) {
            ABSORB(s->b[0], ad, 8);
            i = 1;
            ad += 8;
            adlen -= 8;
        }
        ABSORB(s->b[i], ad, adlen);
        s->b[i][7 - adlen] ^= 0x80;
        P1(s);
    }
    s->b[4][0] ^= 1;
}

// Encrypt plaintext
forceinline void ladybug_encrypt(ladybug_state_t* s, uint8_t* c, const uint8_t* m, uint64_t mlen) {
    const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
    while (mlen >= LADYBUG_AEAD_RATE) {
        ENCRYPT(s->b[0], c, m, 8);
        if (LADYBUG_AEAD_RATE == 16) ENCRYPT(s->b[1], c + 8, m + 8, 8);
        P1(s);
        m += LADYBUG_AEAD_RATE;
        c += LADYBUG_AEAD_RATE;
        mlen -= LADYBUG_AEAD_RATE;
    }
    int i = 0;
    if (LADYBUG_AEAD_RATE == 16 && mlen >= 8) {
        ENCRYPT(s->b[0], c, m, 8);
        i = 1;
        m += 8;
        c += 8;
        mlen -= 8;
    }
    ENCRYPT(s->b[i], c, m, mlen);
    s->b[i][7 - mlen] ^= 0x80;
}

// Define ladybug_decrypt function
forceinline void ladybug_decrypt(ladybug_state_t* s, uint8_t* m, const uint8_t* c, uint64_t clen) {
    const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
    while (clen >= LADYBUG_AEAD_RATE) {
        DECRYPT(s->b[0], m, c, 8);
        if (LADYBUG_AEAD_RATE == 16) DECRYPT(s->b[1], m + 8, c + 8, 8);
        P1(s);
        m += LADYBUG_AEAD_RATE;
        c += LADYBUG_AEAD_RATE;
        clen -= LADYBUG_AEAD_RATE;
    }
    int i = 0;
    if (LADYBUG_AEAD_RATE == 16 && clen >= 8) {
        DECRYPT(s->b[0], m, c, 8);
        i = 1;
        m += 8;
        c += 8;
        clen -= 8;
    }
    DECRYPT(s->b[i], m, c, clen);
    s->b[i][7 - clen] ^= 0x80;
}

// Finalize AEAD
forceinline void ladybug_final(ladybug_state_t* s, const ladybug_key_t* key) {
    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[LADYBUG_AEAD_RATE / 8], key->b[0], 16);
    #endif
    P1(s);
    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[3], key->b[0], 16);
    #endif
}

// ENCRYPTION AEAD
int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k) {
    ladybug_state_t s = {0};  // Zero-initialize the state

    (void)nsec;
    *clen = mlen + CRYPTO_ABYTES;

    // Print the key and nonce before initialization
    printf("Encryption: Key before initialization\n");
    for (int i = 0; i < CRYPTO_KEYBYTES; i++) {
        printf("%02x", k[i]);
    }
    printf("\n");

    printf("Encryption: Nonce before initialization\n");
    for (int i = 0; i < CRYPTO_NPUBBYTES; i++) {
        printf("%02x", npub[i]);
    }
    printf("\n");


    // Print the state before initialization
    // printf("Encryption: State before initialization\n");
    // print_data_byte(&s);

    // Explicitly initialize the state during AEAD initialization
    ladybug_key_t key;
    ladybug_loadkey(&key, k);

    ladybug_initaead(&s, &key, npub);
    // printf("Encryption: State after initialization\n");
    // print_data_byte(&s);

    ladybug_adata(&s, ad, adlen);
    ladybug_encrypt(&s, c, m, mlen);
    ladybug_final(&s, &key);

    // Generate the tag
    SQUEEZE(c + mlen, s.b[3], 8);
    SQUEEZE(c + mlen + 8, s.b[4], 8);
    return 0;
}

// DECRYPTION AEAD
int crypto_aead_decrypt(unsigned char* m, unsigned long long* mlen,
                        unsigned char* nsec, const unsigned char* c,
                        unsigned long long clen, const unsigned char* ad,
                        unsigned long long adlen, const unsigned char* npub,
                        const unsigned char* k) {
    ladybug_state_t s = {0};  // Zero-initialize the state
    (void)nsec;

      // Print the key and nonce before initialization
    printf("Decryption: Key before initialization\n");
    for (int i = 0; i < CRYPTO_KEYBYTES; i++) {
        printf("%02x", k[i]);
    }
    printf("\n");

    printf("Decryption: Nonce before initialization\n");
    for (int i = 0; i < CRYPTO_NPUBBYTES; i++) {
        printf("%02x", npub[i]);
    }
    printf("\n");

    if (clen < CRYPTO_ABYTES) return -1;
    *mlen = clen - CRYPTO_ABYTES;

    // Explicitly initialize the state during AEAD initialization
    ladybug_key_t key;
    ladybug_loadkey(&key, k);

    // printf("Decryption: State before initialization\n");
    // print_data_byte(&s);

    ladybug_initaead(&s, &key, npub);
    // printf("Decryption: State after initialization\n");
    // print_data_byte(&s);

    ladybug_adata(&s, ad, adlen);
    ladybug_decrypt(&s, m, c, *mlen);
    ladybug_final(&s, &key);

    // Verify the tag
    uint8_t r = 0;
    r |= VERIFY(s.b[3], c + *mlen, 8);
    r |= VERIFY(s.b[4], c + *mlen + 8, 8);
    return ((((int)r - 1) >> 8) & 1) - 1;
}

#endif // LADYBUG_AEAD_RATE
