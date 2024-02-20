#include <stdio.h>
#include <stdint.h>
#include "utils.h"

void print_vector(uint64_t *vector, int vector_size){
    for(int i = 0; i < vector_size; i++){
        printf("vector[%d] = %llu\n", i, vector[i]);
    }
}