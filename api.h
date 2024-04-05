#ifndef LADYBUG128V2_API_H  // Fixed the typo here; it should match with the ending #endif directive
#define LADYBUG128V2_API_H  // Also corrected the typo here to match the initial #ifndef directive

#define CRYPTO_VERSION "0.1.0"
#define CRYPT0_KEYBYTES 16 // Key size 16 byte or 128-bit
#define CRYPTO_NSECBYTES 0 // Secret nonce is zero byte as it's only added for compatibility but not used in implementation
#define CRYPTO_NPUBBYTES 16 // Public nonce is 16 byte or 128-bit
#define CRYPTO_ABYTES 16 // Associated data size 16 byte or 128-bit
#define LADYBUG_AEAD_RATE 8 // AEAD rate is 8 byte or 64-bit

#endif // LADYBUG128V2_API_H  // Corrected this comment to match the corrected header guard
