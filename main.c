#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"
#include <string.h> // For strlen

int main() {
    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blocks.";
//    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blo.";
    size_t mlen = strlen((char *)text);
//    for(size_t i = 0; i < mlen; i++){
//        printf("%c", text[i]);
//    }
//    printf("\n");
//    print_vector(text, mlen);
//    printf("mlen = %zu\n", mlen);
    unsigned char c[1024]; // Assuming 1024 is sufficient; adjust based on your needs
    unsigned long long clen;
    unsigned char m[N];  // buffer for decrypted message
    unsigned char nsec;                  // not used, can be NULL or any value
    unsigned long long adlen = 16; // length of the associated data
//    printf("=======================INITIALIZATION=======================\n");
    //GENERATING NONCE
    uint8_t npub[16];

    generate_nonce(npub, 16);
//    printf("128-bit NONCE (N):\n");
//    print_vector(npub, 16);
//    printf("\n");

//EMPTY ASSOCIATED DATA
//    unsigned char ad[34] = {};
    unsigned char ad[34] = "Associated Data can be of any size";
    unsigned char modified_ad[34] = "Associated Data can be of any sizf";
    unsigned long long ad_len = strlen((char *)ad);
    unsigned char k[16] = {0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0x00};
//    printf("128-bit KEY (K):\n");
//    print_vector(k, 16);
//    printf("\n");

    //ENCRYPT AEAD WITH MODIFIED ASSOCIATED DATA
    printf("==================AEAD Encryption==================\n");
    unsigned char* ciphertext;

    crypto_aead_encrypt(c, &clen, text, mlen, ad, ad_len, NULL, npub, k);
    printf("\n");

    printf("CIPHERTEXT:\n");
    print_vector(c, clen);
    //DECRYPTION AEAD
    printf("==================AEAD Decryption==================\n");
    size_t result;

    //DECRYPTION WITH SAME ASSOCIATED DATA
    result = crypto_aead_decrypt(m, &mlen, c, clen, ad, ad_len, NULL, npub, k);
    printf("result = %d\n", result);
    //DECRYPTION WITH MODIFIED ASSOCIATED DATA
//    result = crypto_aead_decrypt(m, &mlen, c, clen, modified_ad, ad_len, NULL, npub, k);
    printf("RECOVERED PLAINTEXT:\n");
//    print_vector(m, mlen);
//    print_character(m, mlen);
    return 0;
}