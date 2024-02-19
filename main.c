#include <stdio.h>
#include <stdint.h> //
#include <string.h>

#define N 8
#define p 7
#define Mp ((1 << p) - 1) // Modulus for encryption operation
#define block_number 5
uint8_t invN = (1 << p) / N;
#define BIT_SIZE (N * 8) // Number of bits in the array

typedef struct state{
    uint64_t x[block_number];
}data_struct;

#define ROUNDS 3
void encryption_round_function(data_struct *state,  uint8_t transform_matrix[N][N]);
void decryption_round_function(data_struct *state,  uint8_t transform_matrix[N][N],  uint8_t inverseN);
void encrypt(data_struct *state, uint8_t transform_matrix[N][N], int rounds);
void decrypt(data_struct *state, uint8_t transform_matrix[N][N], int rounds);
void split_state_into_data_bytes(data_struct *state, uint8_t *data_byte, int block_index);
void combine_data_bytes_to_state(uint8_t *data_byte, data_struct *state, int block_index);
void transpose_ONMNT(uint8_t matrix[N][N]);
void print_vector(uint8_t *vector, int size);
void printMatrix(uint8_t matrix[N][N]);

int main() {
    uint8_t NMNT[N][N] = {{1, 1,   1,   1,   1,   1,   1,   1},
                          {1, 111, 1,   0,   126, 16,  126, 0},
                          {1, 1,   126, 126, 1,   1,   126, 126},
                          {1, 0,   126, 111, 126, 0,   1,   16},
                          {1, 126, 1,   126, 1,   126, 1,   126},
                          {1, 16,  1,   0,   126, 111, 126, 0},
                          {1, 126, 126, 1,   1,   126, 126, 1},
                          {1, 0,   126, 16,  126, 0,   1,  111}};

    uint8_t ONMNT[N][N] = {{1, 1, 1, 1, 1, 1, 1, 1},
                           {82, 82, 3, 124, 45, 45, 124, 3},
                           {111, 0, 16, 0, 111, 0, 16, 0},
                           {82, 45, 3, 3, 45, 82, 124, 124},
                           {1, 126, 1, 126, 1, 126, 1, 126},
                           {3, 3, 45, 82, 124, 124, 82, 45},
                           {0, 111, 0 ,16, 0, 111, 0, 16},
                           {124, 3, 82, 82, 3, 124, 45, 45}};

    uint8_t O2NMNT[N][N] = {{72, 19, 19, 72, 5, 99, 28, 122},
                            {19, 5, 122, 108, 28, 72, 72, 28},
                            {19, 122, 122, 19, 28, 55, 72, 99},
                            {72, 108, 19, 55, 5, 28, 28, 5},
                            {5, 28, 28, 5, 55, 19, 108, 72},
                            {99, 72, 55, 28, 19, 122, 122, 19},
                            {28, 72, 72, 28, 108, 122, 5, 19},
                            {122, 28, 99, 5, 72, 19, 19, 72}};

    data_struct s;
    data_struct t;
    uint8_t data_block_0[N] = "disc0mB0"; //Message block 1
    uint8_t data_block_1[N] = "disc0mB1"; //Message block 2
    uint8_t data_block_2[N] = "disc0mB2"; //Message block 3
    uint8_t data_block_3[N] = "disc0mB3"; //Message block 4
    uint8_t data_block_4[N] = "disc0mB4"; //Message block 5

    //LOAD DATABLOCK FROM STATE
    memcpy(&s.x[0], data_block_0, sizeof(s.x[0]));
    memcpy(&s.x[1], data_block_1, sizeof(s.x[1]));
    memcpy(&s.x[2], data_block_2, sizeof(s.x[2]));
    memcpy(&s.x[3], data_block_3, sizeof(s.x[3]));
    memcpy(&s.x[4], data_block_4, sizeof(s.x[4]));

    printf("PLAINTEXT:\n");
    for(int block_index = 0; block_index < block_number; block_index++){
        printf("BLOCK #%d\n", block_index + 1);
        printf("state: 0x%llx\n", s.x[block_index]);
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(&s, data_byte, block_index);
        print_vector(data_byte, N);
        printf("\n");
    }

    encrypt(&s, O2NMNT, ROUNDS);

    printf("RECOVERED PLAINTEXT:\n");
    //TRANSPOSE MATRIX ONLY FOR ONMNT
    //transpose_ONMNT(ONMNT);
    decrypt(&s, O2NMNT, ROUNDS);
    for(int block_index = 0; block_index < block_number; block_index++){
        printf("BLOCK #%d\n", block_index + 1);
        printf("state: 0x%llx\n", s.x[block_index]);
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(&s, data_byte, block_index);
        print_vector(data_byte, N);
        printf("\n");
    }
    return 0;
}

//ENCRYPTION ROUND FUNCTION
void encryption_round_function(data_struct *state, uint8_t transform_matrix[N][N]) {
    for(int block_index = 0; block_index < 5; block_index++){
        uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
        }

        for(int i = 0; i < N; i++){
            data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
        }
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

//ENCRYPTION OPERATION
void encrypt(data_struct *state, uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
//        printf("round number = %d\n", i+1);
        encryption_round_function(state, transform_matrix); // Corrected passing of state
//        printf("\n");
    }
}

//DECRYPTION ROUND FUNCTION
void decryption_round_function(data_struct *state, uint8_t transform_matrix[N][N], uint8_t inverseN){
    for(int block_index = 0; block_index < block_number; block_index++){
        uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
        uint8_t data_byte[N] = {0};
        split_state_into_data_bytes(state, data_byte, block_index);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < N; ++i) {
                sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
            }
        }
        for(int i = 0; i < N; i++){
            data_byte[i] = (sum_NMNT[i] * inverseN) % Mp;
        }
        state->x[block_index] = 0;
        combine_data_bytes_to_state(data_byte, state, block_index);
    }
}

//DECRYPTION OPERATION
void decrypt(data_struct *state,  uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        decryption_round_function(state, transform_matrix,  invN);
    }
}

//TRANSPOSE ONMNT
void transpose_ONMNT(uint8_t matrix[N][N]){
    uint8_t temp;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

//FUNCTION TO PRINT MATRIX
void printMatrix(uint8_t matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d", matrix[i][j]);
        }
    }
}

void print_vector(uint8_t *vector, int size){
    for(int i = 0; i < size; i++){
        printf("vector[%i] = %d\n", i, vector[i]);
    }
}
void split_state_into_data_bytes(data_struct *state, uint8_t *data_byte, int block_index) {
    // Only work on the specific block at block_index
    for (int i = 0; i < N; ++i) {
        uint8_t byte_offset = 8 * i;
        data_byte[i] = (state->x[block_index] >> byte_offset) & 0xFF;
    }
}

void combine_data_bytes_to_state(uint8_t *data_byte, data_struct *state, int block_index){
        // Reconstruct state->x[0] from data_byte array in little endian format
        state->x[block_index] = 0; // Reset state->x[0]
        for (int i = 0; i < N; i++) {
            uint64_t temp = data_byte[i];
            uint8_t byte_offset = 8 * i;
            state->x[block_index] |= temp << byte_offset;
        }
}


