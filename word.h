#ifndef DIFFUSION_LAYER_WORD_H
#define DIFFUSION_LAYER_WORD_H

#include <stdint.h>
#include <stdlib.h>

//GET BYTE FROM 64-BIT WORD
#define GET_BYTE(x, i) ((uint8_t)((uint64_t)(x) >>(56 - 8 * (i))))

//SET BYTE IN 64-BIT WORD
#define SET_BYTE(b, i) ((uint64_t)(b) << (56 - 8 * (i)))

//SET PADDING BYTE IN 64-BIT WORD
#define PAD(i) SET_BYTE(0x80, i)

//LOAD BYTES INTO 64-BIT ASCON WORD
static inline uint64_t LOAD_BYTES(const uint8_t* bytes, size_t n){
    size_t i;
    uint64_t x = 0;
    for(i = 0; i < n; i++){
        x |= SET_BYTE(bytes[i], i);
    }
    return x;
}

//STORE BYTES FROM 64-BIT WORD
static inline void STORE_BYTES(uint8_t* bytes, uint64_t x, size_t n){
    size_t i;
    for(i = 0; i < n; i++){
        bytes[i] = GET_BYTE(x, i);
    }
}

//CLEAR BYTE IN 64-BIT WORD
static inline uint64_t CLEAR_BYTES(uint64_t x, size_t n){
    size_t i;
    for(i = 0; i < n; i++){
        x &= ~SET_BYTE(0xFF, i);
    }
    return x;
}

#endif //DIFFUSION_LAYER_WORD_H
