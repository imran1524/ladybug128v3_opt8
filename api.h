#ifndef LADYBUG128_API_H
#define LADYBUG128_API_H

#define VERSION "0.1.0"
#define CRYPT0_KEYBYTES 16 //Key size 16 byte or 128-bit
#define CRYPTO_NSECBBYTES 0 //Secret nonce is zero  byte as it's only added for compatibility but not used in implementation
#define CRYPTO_NPUBBYTES 12 //NPUB_BYTES 16 //Public nonce is 16 byte or 128-bit
#define CRYPTO_ABYTES 16 //Associated data size 16 byte or 128-bit
#define RATE 8 //AEAD rate is 8 byte or 64-bit


/*#define CRYPTO_KEYBYTES 32
#define CRYPTO_NSECBYTES 0
#define CRYPTO_NPUBBYTES 12
#define CRYPTO_ABYTES 16
#define CRYPTO_NOOVERLAP 1
 *
 * */


#endif //LADYBUG128_API_H
