#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "ladybug.h"
#include "cJSON/cJSON.h"
#include "crypto_aead.h"

int main(int argc, char* argv[]) {

    const char* file_location = "./test_vector.json";
    long file_size;
    char* json_string = parseJsonFile(file_location, &file_size);

    if (json_string == NULL) {
        fprintf(stderr, "Failed to parse JSON file.\n");
        return 1;
    }

    printf("file_size = %ld\n", file_size);
    cJSON* root = cJSON_Parse(json_string);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON.\n");
        free(json_string);
        return 1;
    }

    cJSON* algorithmItem = cJSON_GetObjectItem(root, "algorithm");
    cJSON* numberOfTestsItem = cJSON_GetObjectItem(root, "numberOfTests");

    if (algorithmItem && cJSON_IsString(algorithmItem)) {
        const char* algorithm = algorithmItem->valuestring;
        printf("algorithm = %s\n", algorithm);
    }

    if (numberOfTestsItem && cJSON_IsNumber(numberOfTestsItem)) {
        const uint64_t numberOfTests = numberOfTestsItem->valueint;
        printf("numberOfTests = %llu\n", numberOfTests);
    }
    cJSON* testGroups = cJSON_GetObjectItem(root, "testGroups");

    if (!testGroups) {
        fprintf(stderr, "testGroups not found.\n");
    } else {
        cJSON* testGroup = NULL;
        cJSON_ArrayForEach(testGroup, testGroups) {
            cJSON* tests = cJSON_GetObjectItem(testGroup, "tests");
            cJSON* ivSizeItem = cJSON_GetObjectItem(testGroup, "ivSize");
            cJSON* keySizeItem = cJSON_GetObjectItem(testGroup, "keySize");

            if (!ivSizeItem) {
                fprintf(stderr, "ivSize is not found.\n");
            } else {
                const uint8_t ivSize = ivSizeItem->valueint;
                printf("ivSize = %d\n", ivSize);
            }

            if (!keySizeItem) {
                fprintf(stderr, "keySizeItem is not found.\n");
            } else {
                const int keySize = keySizeItem->valueint;
                printf("keySize = %d\n", keySize);
            }

            if (!tests) {
                fprintf(stderr, "tests not found.\n");
            } else {
                cJSON* test = NULL;
                cJSON_ArrayForEach(test, tests) {
                    cJSON* key_item = cJSON_GetObjectItem(test, "key");
                    cJSON* iv_item = cJSON_GetObjectItem(test, "iv");
                    cJSON* nonce_item = cJSON_GetObjectItem(test, "nonce");
                    cJSON* aad_item = cJSON_GetObjectItem(test, "aad");
                    cJSON* msg_item = cJSON_GetObjectItem(test, "msg");
                    cJSON* ct_item = cJSON_GetObjectItem(test, "ct");
                    cJSON* tag_item = cJSON_GetObjectItem(test, "tag");

                    printf("\n");

                    if (key_item && cJSON_IsString(key_item)) {
                        const char* key = key_item->valuestring;
                        printf("key = %s\n", key);
                    }
                    if (iv_item && cJSON_IsString(iv_item)) {
                        const char* iv = iv_item->valuestring;
                        printf("iv = %s\n", iv);
                    }

                    if (aad_item && cJSON_IsString(aad_item)) {
                        const char* aad = aad_item->valuestring;
                        printf("AD = %s\n", aad);
                    }
                    if (msg_item && cJSON_IsString(msg_item)) {
                        const char* msg = msg_item->valuestring;
                        printf("msg = %s\n", msg);
                    }
                    if (ct_item && cJSON_IsString(ct_item)) {
                        const char* ct = ct_item->valuestring;
                        printf("ct = %s\n", ct);
                    }
                    if (tag_item && cJSON_IsString(tag_item)) {
                        const char* tag = tag_item->valuestring;
                        printf("tag = %s\n", tag);
                    }

                    // Extract the values from the test case
                    const char* key_str = key_item->valuestring;
                    const char* iv_str = iv_item->valuestring;
                    const char* nonce_str = iv_item->valuestring;
                    const char* aad_str = aad_item->valuestring;
                    const char* msg_str = msg_item->valuestring;
                    const char* ct_str = ct_item->valuestring;
                    const char* tag_str = tag_item->valuestring;

                    unsigned long long key_len = strlen(key_str) / 2;
                    printf("key_len = %llu\n", key_len);

                    unsigned long long aad_len = strlen(aad_str) / 2;
                    printf("aad_len = %llu\n", aad_len);

                    unsigned long long msg_len = strlen(msg_str) / 2;
                    printf("message_len = %llu\n", msg_len);

                    unsigned long long ct_len = strlen(ct_str) / 2;
                    printf("ct_len = %llu\n", ct_len);

                    unsigned long long tag_len = strlen(tag_str) / 2;
                    printf("tag_len = %llu\n", tag_len);

                    unsigned char key[key_len];
                    unsigned char iv[key_len];
                    unsigned char nonce[key_len];
                    unsigned char aad[aad_len];
                    unsigned char msg[msg_len];
                    unsigned char ct[msg_len + tag_len];

                    // Convert the key, IV, AAD, and message strings to binary
                    hex_string_to_binary(key_str, key, key_len);
                    hex_string_to_binary(iv_str, iv, key_len);
                    hex_string_to_binary(nonce_str, nonce, key_len);
                    hex_string_to_binary(aad_str, aad, aad_len);
                    hex_string_to_binary(msg_str, msg, msg_len);

                    // Perform AEAD encryption
                    unsigned long long ciphertext_len = msg_len + tag_len; // Maximum possible ciphertext length
                    unsigned char* ciphertext = (unsigned char*)malloc(ciphertext_len);
                    if (!ciphertext) {
                        fprintf(stderr, "Memory allocation failed.\n");
                        cJSON_Delete(root);
                        free(json_string);
                        return 1;
                    }

                    int encryption_result = crypto_aead_encrypt(
                        ciphertext, &ciphertext_len,
                        msg, msg_len,
                        aad, aad_len,
                        NULL,
                        nonce,
                        key);

                    if (encryption_result != 0) {
                        fprintf(stderr, "Encryption failed.\n");
                        free(ciphertext);
                        cJSON_Delete(root);
                        free(json_string);
                        return 1;
                    }
                    printf("\n");

                    printf("PLAINTEXT:\n");
                    print_vector(msg, msg_len);
                    printf("\n");

                    printf("CIPHERTEXT:\n");
                    print_vector(ciphertext, ciphertext_len);
                    printf("\n");

                    // Perform AEAD decryption
                    int decryption_result = crypto_aead_decrypt(
                        msg, &msg_len,
                        NULL,
                        ciphertext, ciphertext_len,
                        aad, aad_len,
                        nonce, key);
                    if (decryption_result != 0) {
                        printf("Tag verification failed.\n");
                    } else {
                        printf("RECOVERED PLAINTEXT:\n");
                        print_vector(msg, msg_len);
                        printf("\n");
                    }

                    free(ciphertext);
                }
            }
        }
    }

    cJSON_Delete(root);
    free(json_string);

    return 0;
}