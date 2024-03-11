#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"
#include <string.h> // For strlen

int main() {
    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blocks Hello, this is a test string to convert into 64-bit";
    unsigned long long mlen = strlen((char *)text);
//    print_vector(text, mlen);
//    printf("\n");
    unsigned char c[1024]; // Assuming 1024 is sufficient; adjust based on your needs
    unsigned long long clen;
    unsigned char m[N];  // buffer for decrypted message
    unsigned char nsec;                  // not used, can be NULL or any value
    unsigned long long adlen = 16; // length of the associated data
//    print_vector(text, sizeof(text)/sizeof(text[0]));

    //GENERATING NONCE
    uint8_t npub[16];
    generate_nonce(npub, 16);
//    printf("Generating nonce\n");
//    print_vector(npub, 16);
//    printf("\n");
    unsigned char ad[16] = "Associated Data";
    unsigned long long ad_len = strlen((char *)ad);

    unsigned char k[16] = {0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0x00};

    //ENCRYPT AEAD
    printf("==================AEAD Encryption==================\n");
    crypto_aead_encrypt(c, &clen, text, mlen, ad, ad_len, NULL, npub, k);
    printf("\n");
    //DECRYPTION AEAD
    printf("==================AEAD Decryption==================\n");
    crypto_aead_decrypt(m, &mlen, c, clen, ad, adlen, NULL, npub, k);

    return 0;
}
