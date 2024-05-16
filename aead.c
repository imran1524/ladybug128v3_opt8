#include "api.h"
#include "ladybug.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "utils.h"
#include "printstate.h"

#if !LADYBUG_INLINE_MODE
#undef forceinline
#define forceinline
#endif

#ifdef LADYBUG_AEAD_RATE

forceinline void ladybug_state_init(ladybug_state_t* state) {
    printf("I'm inside ladybug_state_init\n");
    memset(state, 0, sizeof(ladybug_state_t));
}

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
        
            s->x[0] = LADYBUG_128_IV;
        }
        memcpy(s->b[1], key->b[0], 16);
    #endif

    if (npub != NULL) {
      
        INSERT(s->b[3], npub, 8);
        INSERT(s->b[4], npub + 8, 8);
    } else {
        return;
    }
      printf("I'm inside ladybug_initaead\n");
    printf("Print state before permutation:\n");
    print_data_byte(s);
    P1(s);
    printf("Print state after permutation:\n");
    print_data_byte(s);

    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[3], key->b[0], 16);
        printstate("init 2nd key xor", s);
    #endif
}




//Force inline function to process the associated data
forceinline void ladybug_adata(ladybug_state_t* s, const uint8_t* ad, uint64_t adlen) {
    const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
    if (adlen) {
        while (adlen >= LADYBUG_AEAD_RATE) {
            printf("I'm inside ladybug_adata\n");
            ABSORB(s->b[0], ad, 8);
            if (LADYBUG_AEAD_RATE == 8) ABSORB(s->b[1], ad + 8, 8);
            printstate("absorb adata", s);
            printf("Print state before permutation:\n");
            print_data_byte(s);
            P1(s);
            printf("Print state after permutation:\n");
            print_data_byte(s);
            ad += LADYBUG_AEAD_RATE;
            adlen -= LADYBUG_AEAD_RATE;
        }

        int i = 0;
        if (LADYBUG_AEAD_RATE == 8 && adlen >= 8) {
            ABSORB(s->b[0], ad, 8);
            i = 1;
            ad += 8;
            adlen -= 8;
        }

        ABSORB(s->b[i], ad, adlen);
        s->b[i][7 - adlen] ^= 0x80;
        printstate("pad adata", s);
        P1(s);
    }

    s->b[4][0] ^= 1;
    printstate("domain separation", s);
}


//Force inline function to encrypt the plaintext
forceinline void ladybug_encrypt(ladybug_state_t* s, uint8_t* c, const uint8_t* m, uint64_t mlen) {
    printf("mlen");
    while (mlen >= LADYBUG_AEAD_RATE) {

        printf("I'm inside ladybug_encrypt\n");
        ENCRYPT(s->b[0], c, m, 8);
        if (LADYBUG_AEAD_RATE == 8) ENCRYPT(s->b[1], c + 8, m + 8, 8);
        printf("State before permutation:\n");
        print_state(s);
        P1(s);
        printf("State after permutation:\n");
        print_state(s);
        m += LADYBUG_AEAD_RATE;
        c += LADYBUG_AEAD_RATE;
        mlen -= LADYBUG_AEAD_RATE;
    }

    int i = 0;
    if (LADYBUG_AEAD_RATE == 8 && mlen >= 8) {
        ENCRYPT(s->b[0], c, m, 8);
        i = 1;
        m += 8;
        c += 8;
        mlen -= 8;
    }
    ENCRYPT(s->b[i], c, m, mlen);
    s->b[i][7 - mlen] ^= 0x80;
    printstate("pad plaintext", s);
}

forceinline void ladybug_decrypt(ladybug_state_t* s, uint8_t* m, const uint8_t* c, uint64_t clen) {
  // const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
  /* full ciphertext blocks */
  while (clen >= LADYBUG_AEAD_RATE) {
    DECRYPT(s->b[0], m, c, 8);
    if (LADYBUG_AEAD_RATE == 16) DECRYPT(s->b[1], m + 8, c + 8, 8);
    printstate("insert ciphertext", s);
    // P(s, nr);
    P1(s);
    m += LADYBUG_AEAD_RATE;
    c += LADYBUG_AEAD_RATE;
    clen -= LADYBUG_AEAD_RATE;
  }

  /* final ciphertext block */
  int i = 0;
  if (LADYBUG_AEAD_RATE == 8 && clen >= 8) {
    DECRYPT(s->b[0], m, c, 8);
    i = 1;
    m += 8;
    c += 8;
    clen -= 8;
  }

  DECRYPT(s->b[i], m, c, clen);
  s->b[i][7 - clen] ^= 0x80;
  printstate("pad ciphertext", s);
}

forceinline void ladybug_final(ladybug_state_t* s, const ladybug_key_t* key) {
    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[LADYBUG_AEAD_RATE / 8], key->b[0], 16);
    #endif

    printstate("final 1st key xor", s);
    P1(s);

    #if CRYPTO_KEYBYTES == 16
        memxor(s->b[3], key->b[0], 16);
    #endif
    printstate("final 2nd key xor", s);
}


//ENCRYPTION AEAD
int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen, const unsigned char* m, unsigned long long mlen, const unsigned char* ad, unsigned long long adlen, const unsigned char* nsec, const unsigned char* npub, const unsigned char* k) {
    ladybug_state_t s;
    (void)nsec;
    *clen = mlen + CRYPTO_ABYTES;

    ladybug_state_init(&s);
    // printf("Encryption: State initialized:\n");
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

    SQUEEZE(c + mlen, s.b[3], 8);
    SQUEEZE(c + mlen + 8, s.b[4], 8);
    return 0;
}


//DECRYPTION AEAD
int crypto_aead_decrypt(unsigned char* m, unsigned long long* mlen, unsigned char* nsec, const unsigned char* c, unsigned long long clen, const unsigned char* ad, unsigned long long adlen, const unsigned char* npub, const unsigned char* k) {
    ladybug_state_t s;
    (void)nsec;

    if (clen < CRYPTO_ABYTES) {
        return -1;
    }

    *mlen = clen - CRYPTO_ABYTES;

    ladybug_state_init(&s);
    // printf("Decryption: State initialized:\n");
    // print_data_byte(&s);

    ladybug_key_t key;
    ladybug_loadkey(&key, k);
    // printf("Decryption: Key loaded:\n");
    // print_data_byte(&key);

    ladybug_initaead(&s, &key, npub);
    // printf("Decryption: State after initialization:\n");
    // print_data_byte(&s);

    ladybug_adata(&s, ad, adlen);
    // printf("Decryption: State after processing associated data:\n");
    // print_data_byte(&s);

    ladybug_decrypt(&s, m, c, *mlen);
    // printf("Decryption: State after decryption:\n");
    // print_data_byte(&s);

    ladybug_final(&s, &key);
    // printf("Decryption: State after finalization:\n");
    // print_data_byte(&s);

    uint8_t r = 0;
    r |= VERIFY(s.b[3], c + *mlen, 8);
    r |= VERIFY(s.b[4], c + *mlen + 8, 8);
    return ((((int)r - 1) >> 8) & 1) - 1;
}

    
#endif // LADYBUG_AEAD_RATE`