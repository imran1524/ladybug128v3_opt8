#include "utils.h"
#include "permutations.h"
#include "round.h"

int main() {
    char text[] = "Hello, this is a test string to convert into 64-bit blocks!";
    data_struct s; // Initialize the data_struct

    text_to_64bit_blocks_data_struct(text, &s);  // Assuming this function is updated to take a pointer
//    forward_permutation(&s);
//    inverse_permutation(&s);

    FP3(&s);
    IP3(&s);

    return 0;
}

