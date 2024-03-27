#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"
#include <string.h> // For strlen
#include "cJSON/cJSON.h"

int main() {
    long file_size;
    char* file_location = "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/custom_test_vector.json";
    char* json_string = parseJsonFile(file_location, &file_size);

    if (json_string == NULL) {
        fprintf(stderr, "Failed to parse JSON file.\n");
        return -1;
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

                //                    "tcId": 1,
//                            "comment": "",
//                            "key": "00000000000000000000000000000000",
//                            "iv": "00000000000000000000000000000000",
//                            "nonce": "00000000000000000000000000000000",
//                            "aad": "",
//                            "msg": "",
//                            "ct": "",
//                            "tag": "0f3a56aa8add61cae8aa70b51b0dbc6b",
//                            "result": "valid",
//                            "flags": []

                cJSON* test = NULL;
                cJSON_ArrayForEach(test, tests) {
                    cJSON* key_item = cJSON_GetObjectItem(test, "key");
                    cJSON* iv_item= cJSON_GetObjectItem(test, "iv");
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

                    if(nonce_item){

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
                    const char* key_str = key_item -> valuestring;
                    const char* iv_str = iv_item -> valuestring;
                    const char* nonce_str = iv_item -> valuestring;
                    const char* aad_str = aad_item -> valuestring;
                    const char* msg_str = msg_item -> valuestring;
                    const char* ct_str = ct_item -> valuestring;
                    const char* tag_str = tag_item->valuestring;


                    size_t key_len = strlen(key_str) / 2;
                    printf("key_len = %zu\n", key_len);

                    size_t iv_len = strlen(iv_str) / 2;
                    printf("iv_len = %zu\n", iv_len);

                    size_t nonce_len = strlen(nonce_str) / 2;
                    printf("nonce_len = %zu\n", nonce_len);

                    size_t aad_len = strlen(aad_str) / 2;
                    printf("aad_len = %zu\n", aad_len);

                    size_t msg_len = strlen(msg_str) / 2;
                    printf("message_len = %zu\n", msg_len);

                    size_t ct_len = strlen(ct_str) / 2;
                    printf("ct_len = %zu\n", ct_len);

                    size_t tag_len = strlen(tag_str) / 2;
                    printf("tag_len = %zu\n", tag_len);

                    unsigned char key[key_len];
                    unsigned char iv[iv_len];
                    unsigned char nonce[nonce_len];
                    unsigned char aad[aad_len];
                    unsigned char msg[msg_len];
                    unsigned char tag[tag_len];
                    unsigned char ct[msg_len + tag_len];

                    // Convert the key, IV, AAD, and message strings to binary
                    hex_string_to_binary(key_str, key, key_len);
                    hex_string_to_binary(iv_str, iv, iv_len);
                    hex_string_to_binary(nonce_str, nonce, nonce_len);
                    hex_string_to_binary(tag_str, tag, tag_len);
                    hex_string_to_binary(aad_str, aad, aad_len);
                    hex_string_to_binary(msg_str, msg, msg_len);

                    // Perform AEAD encryption
                    size_t max_ciphertext_len = msg_len + tag_len; // Maximum possible ciphertext length
                    unsigned char* ciphertext = (unsigned char*)malloc(max_ciphertext_len);
                    if (!ciphertext) {
                        fprintf(stderr, "Memory allocation failed.\n");
                        cJSON_Delete(root);
                        free(json_string);
                        return 1;
                    }
                    unsigned long long ciphertext_len;

                    /*int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k)
                     * */

                    int encryption_result = crypto_aead_encrypt(ciphertext, &ciphertext_len, msg, msg_len, aad, aad_len, NULL, nonce, key);
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

                    size_t decrypted_len = msg_len;
                    int decryption_result = crypto_aead_decrypt(msg, &decrypted_len, ciphertext, ciphertext_len, aad, aad_len, NULL, nonce, key);
                    if (decryption_result != 0) {
                        printf("Tag verification failed.\n");
                    } else {
                        printf("RECOVERED PLAINTEXT:\n");
                        print_vector(msg, decrypted_len);
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