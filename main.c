#include <stdio.h>
#include <stdint.h> //
#include <string.h>

#define N 8
#define p 7
#define Mp ((1 << p) - 1) // Modulus for encryption operation
uint8_t invN = (1 << p) / N;
#define BIT_SIZE (N * 8) // Number of bits in the array

typedef struct state{
    uint64_t x[1];
}data_struct;

#define ROUNDS 10
void encryption_round_function(data_struct *state,  uint8_t transform_matrix[N][N]);
void decryption_round_function(data_struct *state,  uint8_t transform_matrix[N][N],  uint8_t inverseN);
void encrypt(data_struct *state, uint8_t transform_matrix[N][N], int rounds);
void decrypt(data_struct *state, uint8_t transform_matrix[N][N], int roundsvoid);
void split_state_into_data_bytes(data_struct *state, uint8_t *data_byte);
void combine_data_bytes_to_state(uint8_t *data_byte, data_struct *state);
void transpose_ONMNT(uint8_t matrix[N][N]);

int main() {
    data_struct s;
    data_struct t;
    uint8_t data[N] = "disc0mBo"; // Example data as an array
    memcpy(&s.x[0], data, sizeof(s.x[0]));

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

    printf("\n");
    //ENCRYPTION
    printf("Encryption:\n");
    encrypt(&s, ONMNT, ROUNDS);
    printf("Updated state: 0x%llx\n", s.x[0]);
    printf("\n");

    //DECRYPTION
    printf("Decryption:\n");
    transpose_ONMNT(ONMNT);
    decrypt(&s, ONMNT, ROUNDS);
    printf("Updated state: 0x%llx\n", s.x[0]);


//    printf("\n");
//    printMatrix(ONMNT);


    return 0;
}

//ENCRYPTION ROUND FUNCTION
void encryption_round_function(data_struct *state, uint8_t transform_matrix[N][N]) {
    uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
    uint8_t data_byte[N] = {0};

    //PRINTING STATE
    printf("state before encryption: 0x%llx\n", state->x[0]);

    //CALLING THE FUNCTION TO SPLIT THE 64-BIT STATE INTO 8 X 8-BIT DATA BYTE IN LITTLE ENDIAN WAY
    split_state_into_data_bytes(state, data_byte);
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
//            printf("data_byte[%d] = %d\n",i,  data_byte[i]);
//            printf("transform_matrix[%d][%d] = %d\n", i, j, transform_matrix[i][j]);
            sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
        }
    }
    printf("\n");
    for(int i = 0; i < N; i++){
        data_byte[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
        printf("ciphertext[%d] = %d\n", i,  data_byte[i]);
    }

    //CALLING THE FUNCTION TO COMBINE 8 8-BIT DATA BYTES INTO 64-BIT STATE IN LITTLE ENDIAN WAY
    combine_data_bytes_to_state(data_byte, state);
}

//ENCRYPTION OPERATION
void encrypt(data_struct *state, uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        printf("round number = %d\n", i+1);
        encryption_round_function(state, transform_matrix); // Corrected passing of state
        printf("\n");
    }
}

//DECRYPTION ROUND FUNCTION
void decryption_round_function(data_struct *state, uint8_t transform_matrix[N][N], uint8_t inverseN){
    uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero
    uint8_t data_byte[N] = {0};
    split_state_into_data_bytes(state, data_byte);
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
//            printf("data_byte[%d] = %d\n", i, data_byte[i]);
//            printf("transform_matrix[%d][%d] = %d\n",i, j, transform_matrix[i][j]);
            sum_NMNT[j] += data_byte[i] * transform_matrix[i][j];
        }
//        printf("\n");
    }

    for(int i = 0; i < N; i++){
        data_byte[i] = (sum_NMNT[i] * inverseN) % Mp;
        printf("recovered_plaintext[%d] = %d\n", i, data_byte[i]);
    }

    state->x[0] = 0;
    combine_data_bytes_to_state(data_byte, state);
}

//DECRYPTION OPERATION
void decrypt(data_struct *state,  uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        printf("round number = %d\n", i+1);
        decryption_round_function(state, transform_matrix,  invN);
        printf("\n");
    }
}

//TRANSPOSE ONMNT
void transpose_ONMNT(uint8_t matrix[N][N]){
    uint8_t temp;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            // Swap element [i][j] with element [j][i]
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
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void split_state_into_data_bytes(data_struct *state, uint8_t *data_byte){
    for (int i = 0; i < N; ++i) {
        uint8_t byte_offset = 8 * i; // Corrected byte offset calculation
        data_byte[i] = (state->x[0] >> byte_offset) & 0xFF;
    }

    //PRINTING THE DATA BYTES
    for (int i = 0; i < N; ++i) {
//        printf("data_byte[%d] = %d\n", i, data_byte[i]);
    }
}

void combine_data_bytes_to_state(uint8_t *data_byte, data_struct *state){
    // Reconstruct state->x[0] from data_byte array in little endian format
    state->x[0] = 0; // Reset state->x[0]
    for (int i = 0; i < N; i++) {
        uint64_t temp = data_byte[i];
        // For little endian, we start shifting from 0 for the first byte
        uint8_t byte_offset = 8 * i;
        state->x[0] |= temp << byte_offset;
    }
    printf("state after encryption: 0x%llx\n", state->x[0]);
}

