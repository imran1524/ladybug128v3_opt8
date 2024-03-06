#ifndef DIFFUSION_LAYER_API_H
#define DIFFUSION_LAYER_API_H

#define VERSION "0.1.0"
#define KEY_BYTES 16 //Key size 16 byte or 128-bit
#define NSEC_BYTES 0 //Secret nonce is zero  byte as it's only added for compatibility but not used in implementation
#define NPUB_BYTES 16 //Public nonce is 16 byte or 128-bit
#define AD_BYTES 16 //Associated data size 16 byte or 128-bit
#define RATE 8 //AEAD rate is 8 byte or 64-bit


#endif //DIFFUSION_LAYER_API_H
