#include "api.h"
#include "ladybug.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "utils.h"
#include "printstate.h"

#if !ASCON_INLINE_MODE
#undef forceinline
#define forceinline
#endif

#ifdef LADYBUG_AEAD_RATE

//Force inline function to load the key
forceinline void ladybug_loadkey(ladybug_key_t* key, const uint8_t* k) {
  #if CRYPTO_KEYBYTES == 16
    INSERT(key->b[0], k, 8);
    INSERT(key->b[1], k + 8, 8);
  #else /* CRYPTO_KEYBYTES == 20 */
    key->x[0] = KEYROT(0, LOADBYTES(k, 4));
    key->x[1] = LOADBYTES(k + 4, 8);
    key->x[2] = LOADBYTES(k + 12, 8);
  #endif
}

//Force inline function to initialize the AEAD
forceinline void ladybug_initaead(ladybug_state_t* s, const ladybug_key_t* key,
                                const uint8_t* npub) {
  #if CRYPTO_KEYBYTES == 16
    if (LADYBUG_AEAD_RATE == 8) s->x[0] = LADYBUG_128_IV;
    memcpy(s->b[1], key->b[0], 16);
  #else /* CRYPTO_KEYBYTES == 20 */
    s->x[0] = key->x[0] ^ ASCON_80PQ_IV;
    memcpy(s->b[1], key->b[1], 16);
  #endif

  INSERT(s->b[3], npub, 8);
  INSERT(s->b[4], npub + 8, 8);
  printstate("init 1st key xor", s);

  #if CRYPTO_KEYBYTES == 16
    memxor(s->b[3], key->b[0], 16);
  #else /* CRYPTO_KEYBYTES == 20 */
    memxor(s->b[2], key->b[0], 24);
  #endif
    printstate("init 2nd key xor", s);
}

//Force inline function to process the associated data
forceinline void ladybug_adata(ladybug_state_t* s, const uint8_t* ad, uint64_t adlen) {
  const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
  if (adlen) {
    /* full associated data blocks */
    while (adlen >= LADYBUG_AEAD_RATE) {
      ABSORB(s->b[0], ad, 8);
      if (LADYBUG_AEAD_RATE == 16) ABSORB(s->b[1], ad + 8, 8);
      printstate("absorb adata", s);
      // P(s, nr);
      P1(s);
      ad += LADYBUG_AEAD_RATE;
      adlen -= LADYBUG_AEAD_RATE;
    }
    
    /* final associated data block */
    int i = 0;
    if (LADYBUG_AEAD_RATE == 16 && adlen >= 8) {
      ABSORB(s->b[0], ad, 8);
      i = 1;
      ad += 8;
      adlen -= 8;
    }

    ABSORB(s->b[i], ad, adlen);
    s->b[i][7 - adlen] ^= 0x80;
    printstate("pad adata", s);
    // P(s, nr);
    P1(s);
  }

  /* domain separation */
  s->b[4][0] ^= 1;
  printstate("domain separation", s);
}

//Force inline function to encrypt the plaintext
forceinline void ladybug_encrypt(ladybug_state_t* s, uint8_t* c, const uint8_t* m, uint64_t mlen){
  const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;

  /* full plaintext blocks */
  while (mlen >= LADYBUG_AEAD_RATE) {
    ENCRYPT(s->b[0], c, m, 8);
    if (LADYBUG_AEAD_RATE == 16) ENCRYPT(s->b[1], c + 8, m + 8, 8);
    printstate("absorb plaintext", s);
    // P(s, nr);
    P1(s);
    m += LADYBUG_AEAD_RATE;
    c += LADYBUG_AEAD_RATE;
    mlen -= LADYBUG_AEAD_RATE;
  }

  /* final plaintext block */
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
  printstate("pad plaintext", s);
}

forceinline void ladybug_decrypt(ladybug_state_t* s, uint8_t* m, const uint8_t* c, uint64_t clen) {
  const int nr = (LADYBUG_AEAD_RATE == 8) ? 6 : 8;
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
  if (LADYBUG_AEAD_RATE == 16 && clen >= 8) {
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

forceinline void ladybug_final(ladybug_state_t* s, const ladybug_key_t* key){
  #if CRYPTO_KEYBYTES == 16
    memxor(s->b[LADYBUG_AEAD_RATE / 8], key->b[0], 16);
  #else /* CRYPTO_KEYBYTES == 20 */
    s->x[1] ^= KEYROT(key->x[0], key->x[1]);
    s->x[2] ^= KEYROT(key->x[1], key->x[2]);
    s->x[3] ^= KEYROT(key->x[2], 0);
  #endif

  printstate("final 1st key xor", s);
  // P(s, 12);
  P1(s);
    
  #if CRYPTO_KEYBYTES == 16
    memxor(s->b[3], key->b[0], 16);
  #else /* CRYPTO_KEYBYTES == 20 */
    memxor(s->b[3], key->b[1], 16);
  #endif
    printstate("final 2nd key xor", s);
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
  /* perform ascon computation */
  ladybug_key_t key;
  ladybug_loadkey(&key, k);
  
  ladybug_initaead(&s, &key, npub);
  printf("State after initialization:\n");
  print_data_byte(&s);

  ladybug_adata(&s, ad, adlen);
  printf("State after processing associated data:\n");
  print_data_byte(&s);

  ladybug_encrypt(&s, c, m, mlen);
  printf("State after encryption:\n");
  print_data_byte(&s);

  ladybug_final(&s, &key);
  printf("State after finalization:\n");
  print_data_byte(&s);

    /* set tag */
  SQUEEZE(c + mlen, s.b[3], 8);
  SQUEEZE(c + mlen + 8, s.b[4], 8);
  return 0;
}

//DECRYPTION AEAD
int crypto_aead_decrypt(
            unsigned char *m, unsigned long long *mlen,
            unsigned char *nsec, const unsigned char *c,
            unsigned long long clen, const unsigned char *ad, 
            unsigned long long adlen, const unsigned char *npub,
            const unsigned char *k ){

  ladybug_state_t s;
  (void)nsec;

  if(clen < CRYPTO_ABYTES){
    return -1;
  }

  //SET PLAINTEXT SIZE
  *mlen = clen - CRYPTO_ABYTES;

  //Perform ladybug computation
  ladybug_key_t key;
  ladybug_loadkey(&key, k);

  ladybug_initaead(&s, &key, npub);
  printf("State after initialization:\n");
  print_data_byte(&s);
  
  ladybug_adata(&s, ad, adlen);
  printf("State after processing associated data:\n");
  print_data_byte(&s);
  
  ladybug_decrypt(&s, m, c, *mlen);
  printf("State after decryption:\n");
  print_data_byte(&s);

  ladybug_final(&s, &key);
  printf("State after finalization:\n");
  print_data_byte(&s);

  //VERIFY TAG
  uint8_t r = 0;
  r |= VERIFY(s.b[3], c + clen, 8);
  r |= VERIFY(s.b[4], c + clen + 8, 8);
  return ((((int)r - 1) >> 8) & 1) - 1;
}
    
#endif // LADYBUG_AEAD_RATE