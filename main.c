#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"

int main() {
    char text[] = "Hello, this is a test string to convert into 64-bit blocks!";
    state_t s; // Initialize the data_struct

    text_to_64bit_blocks_data_struct(text, &s);  // Assuming this function is updated to take a pointer
//    forward_permutation(&s);
//    inverse_permutation(&s);

    FP1(&s);
    IP1(&s);

    return 0;
}

