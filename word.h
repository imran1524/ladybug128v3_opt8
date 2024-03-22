#ifndef DIFFUSION_LAYER_WORD_H
#define DIFFUSION_LAYER_WORD_H

#include <stdint.h>
#include <stdlib.h>

/* Define word related operations */

// GET BYTE FROM 64-BIT WORD
// In little endian, the byte order is reversed.
#define GET_BYTE(x, i) ((uint8_t)((uint64_t)(x) >> (8 * (i))))

// SET BYTE IN 64-BIT WORD
// Shift the byte to its correct position in little endian.
#define SET_BYTE(b, i) ((uint64_t)(b) << (8 * (i)))

// SET PADDING BYTE IN 64-BIT WORD
// For padding, the position of the byte is adjusted for little endian.
#define PAD(i) SET_BYTE(0x80, i)

// LOAD BYTES INTO 64-BIT WORD
// Adjusted for little endian: bytes are loaded in reverse order.
static inline uint64_t LOAD_BYTES(const uint8_t* bytes, size_t n) {
    size_t i;
    uint64_t x = 0;
    for (i = 0; i < n; i++) {
        x |= SET_BYTE(bytes[i], i);
    }
    return x;
}

// STORE BYTES FROM 64-BIT WORD
// Adjusted for little endian: bytes are stored in reverse order.
static inline void STORE_BYTES(uint8_t* bytes, uint64_t x, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        bytes[i] = GET_BYTE(x, i);
    }
}

// CLEAR BYTE IN 64-BIT WORD
// Adjusted for little endian: clear bytes in reverse order.
static inline uint64_t CLEAR_BYTES(uint64_t x, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        x &= ~SET_BYTE(0xFF, i);
    }
    return x;
}

#endif // DIFFUSION_LAYER_WORD_H


