#include <stdio.h>
#include <stdint.h> // For uint8_t and uint64_t

#define N 8
#define p 7
#define Mp ((1 << p) - 1) // Modulus for encryption operation
uint8_t invN = (1 << p) / N;
#define BIT_SIZE (N * 8) // Number of bits in the array

void encryption_round_function(uint8_t *data,  uint8_t transform_matrix[N][N]);
void decryption_round_function(uint8_t *data,  uint8_t transform_matrix[N][N],  uint8_t inverseN);
void encrypt(uint8_t *plaintext, uint8_t transform_matrix[N][N], int rounds);
void decrypt(uint8_t *ciphertext, uint8_t transform_matrix[N][N], int rounds);
void transpose_ONMNT(uint8_t matrix[N][N]);
void print_matrix(uint8_t matrix[N][N]);
void calculate_diffusion_percentage(uint8_t *data, size_t size, uint8_t transform_matrix[N][N], int rounds);

int main() {
    uint8_t data[N] = "disc0mBo"; // Example data as an array
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

    int rounds = 10; // Number of encryption rounds

    printf("Original: \n");
    for(int i = 0; i < N; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }
    printf("\n");
    //ENCRYPTION
    encrypt(data, O2NMNT, rounds);
    printf("\n");
    printf("Encrypted: \n");
    for(int i = 0; i < N; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }
//    printf("\n");
//    printMatrix(ONMNT);
    transpose_ONMNT(NMNT);
    //PRINTING TRANSPOSE
    printf("\n");
//    printMatrix(ONMNT);

    decrypt(data, O2NMNT, rounds);
    printf("\n");
    printf("Decrypted: \n");
    for(int i = 0; i < N; i++) {
        printf("recovered[%d] = %d\n", i, data[i]);
    }
    return 0;
}

//ENCRYPTION ROUND FUNCTION
void encryption_roundFunction(uint8_t *data, uint8_t transform_matrix[N][N]) {
    uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero

    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
//            printf("data[%d] = %d\n", i, data[i]);
//            printf("transform_matrix[%d][%d] = %d\n", i, j, transform_matrix[i][j]);
            sum_NMNT[j] += data[i] * transform_matrix[i][j];
        }
    }

    for(int i = 0; i < N; i++){ // Corrected loop condition
        data[i] = sum_NMNT[i] % Mp; // Apply modulus and update data
        printf("ciphertext[%d] = %d\n", i, data[i]);
    }
}

//ENCRYPTION OPERATION
void encrypt(uint8_t *plaintext, uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        printf("round number = %d\n", i+1);
        encryption_roundFunction(plaintext, transform_matrix); // Apply round function
        printf("\n");
    }
}

//DECRYPTION ROUND FUNCTION
void decryption_round_function(uint8_t *data,  uint8_t transform_matrix[N][N],  uint8_t inverseN) {
    uint64_t sum_NMNT[N] = {0}; // Initialize sum array to zero

    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
//            printf("data[%d] = %d\n", i, data[i]);
//            printf("transform_matrix[%d][%d] = %d\n",i, j, transform_matrix[i][j]);
            sum_NMNT[j] += data[i] * transform_matrix[i][j];
        }
//        printf("\n");
    }

    for(int i = 0; i < N; i++){
        data[i] = (sum_NMNT[i] * inverseN) % Mp;
        printf("data[%d] = %d\n", i, data[i]);
    }
}

//DECRYPTION OPERATION
void decrypt(uint8_t *ciphertext,  uint8_t transform_matrix[N][N], int rounds) {
    for (int i = 0; i < rounds; ++i) {
        printf("round number = %d\n", i+1);
        decryption_round_function(ciphertext, transform_matrix,  invN);
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
