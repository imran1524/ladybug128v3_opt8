#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"
#include <string.h> // For strlen

int main() {
    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blocks!";
    unsigned long long mlen = strlen((char *)text);
    unsigned char c[1024]; // Assuming 1024 is sufficient; adjust based on your needs
    unsigned long long clen;

    print_vector(text, sizeof(text)/sizeof(text[0]));

    //GENERATING NONCE
    uint8_t npub[16];
    generate_nonce(npub, 16);
//    printf("Generating nonce\n");
//    print_vector(npub, 16);
    printf("\n");
    unsigned char ad[] = "Associated Data";
    unsigned long long ad_len = strlen((char *)ad);

    unsigned char k[16] = {0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0x00};

    //ENCRYPT AEAD
    crypto_aead_encrypt(c, &clen, text, mlen, ad, ad_len, NULL, npub, k);


    return 0;
}
