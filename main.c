#include "utils.h"
#include "permutations.h"
#include "round.h"
#include "aead.h"
#include <string.h> // For strlen
#include "cJSON/cJSON.h"

int main() {
    long file_size;
    char * file_location = "/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/aegis128_test.json";
    char* json_string = parseJsonFile(file_location, &file_size);

    if(json_string == NULL) {
        return 1;
    }

    printf("file_size = %ld\n", file_size);
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        printf("Failed to parse JSON\n");
        free(json_string);
        return 1;
    }else{
        cJSON *algorithmItem = cJSON_GetObjectItem(root, "algorithm");
        cJSON *numberOfTestsItem = cJSON_GetObjectItem(root, "numberOfTests");

        if(algorithmItem && cJSON_IsString(algorithmItem)){
            const char *algorithm = algorithmItem -> valuestring;
            printf("algorithm = %s\n", algorithm);
        }

        if(numberOfTestsItem && cJSON_IsNumber( numberOfTestsItem)){
            const uint64_t numberOfTests = numberOfTestsItem -> valueint;
            printf("numberOfTests = %llu\n", numberOfTests);
        }
    }


    cJSON *testGroups = cJSON_GetObjectItem(root, "testGroups");

    if (!testGroups) {
        printf("testGroups not found.\n");
    } else {
        cJSON *testGroup = NULL;
        cJSON_ArrayForEach(testGroup, testGroups) {
            cJSON *tests = cJSON_GetObjectItem(testGroup, "tests");
            cJSON *ivSizeItem = cJSON_GetObjectItem(testGroup, "ivSize");
            cJSON *keySizeItem = cJSON_GetObjectItem(testGroup, "keySize");

            if(!ivSizeItem){
                printf("ivSize is not found\n");
            }else{
                const uint8_t ivSize = ivSizeItem -> valueint;
                printf("ivSize = %d\n", ivSize);
            }

            if(!keySizeItem){
                printf("keySizeItem is not found\n");

            }else{
                const int keySize = keySizeItem -> valueint;
                printf("keySize = %d\n", keySize);
            }

            if (!tests) {
                printf("tests not found.\n");
            } else {
                cJSON *test = NULL;
                cJSON_ArrayForEach(test, tests) {
//                    printTestCase(test);
                    cJSON *keyItem = cJSON_GetObjectItem(test, "key");
                    cJSON *ivItem = cJSON_GetObjectItem(test, "iv");
                    cJSON *aadItem = cJSON_GetObjectItem(test, "aad");
                    cJSON *msgItem = cJSON_GetObjectItem(test, "msg");
                    cJSON *ctItem = cJSON_GetObjectItem(test, "ct");
                    cJSON *tagItem = cJSON_GetObjectItem(test, "tag");

#if 1
                    if(keyItem  && cJSON_IsString(keyItem)){
                        const char* key = keyItem -> valuestring;
                        printf("key = %s\n", key);
                    }
                    if(ivItem && cJSON_IsString(ivItem)){
                        const char *iv = ivItem->valuestring;
                        printf("iv = %s\n", iv);
                    }
                    if(aadItem && cJSON_IsString(aadItem)){
                        const char *aad = aadItem->valuestring;
                        printf("AD= %s\n", aad);
                    }
                    if(msgItem && cJSON_IsString(msgItem)){
                        const char *msg = msgItem -> valuestring;
                        printf("msg = %s\n", msg);
                    }
                    if(ctItem && cJSON_IsString(ctItem)){
                        const char *ct = ctItem -> valuestring;
                        printf("ct = %s\n", ct);
                    }
                    if(tagItem && cJSON_IsString(tagItem) ){
                        const char *tag = tagItem -> valuestring;
                        printf("tag = %s\n", tag);
                    }


                    //Extract the values from the test case
                    const char* key_str = keyItem -> valuestring;
                    const char* iv_str = ivItem -> valuestring;
                    const char* aad_str = aadItem -> valuestring;
                    const char* msg_str = msgItem -> valuestring;
                    const char *ct_str = ctItem->valuestring;
                    const char *tag_str = tagItem->valuestring;

                    size_t  key_len = strlen(key_str)/2;
                    printf("key_len = %zu\n", key_len);

                    size_t iv_len = strlen(iv_str)/2;
                    printf("iv_len = %zu\n", iv_len);

                    size_t aad_len = strlen(aad_str)/2;
                    printf("aad_len = %zu\n", aad_len);

//                    printf("key_length = %d\n", keySize);
//                    printf("iv_length = %d\n", ivSize);
//                    unsigned char key[key_len];
//                    unsigned char iv[iv_len];

//                    const char* hex_str = "0123456789ABCDEF0123456789ABCDEF";

                    size_t mgr_len = strlen(msg_str)/2;
                    printf("message_len = %zu\n", mgr_len);

//                    unsigned char binary[binary_len];
//                    hex_string_to_binary(hex_str, binary, binary_len);
//                    printf("Binary string:\n");
//                    print_binary(binary, binary_len);
                    printf("\n");
#endif
                }
            }
        }
    }
    cJSON_Delete(root);
    free(json_string);

    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blocks.";
//    unsigned char text[] = "Hello, this is a test string to convert into 64-bit blo.";

    //CHECK ERROR FOR FILE OPENING
//    if(file == NULL){
//        perror("Failed to open the file\n");
//        return EXIT_FAILURE;
//    }

    char line_buffer[RATE];
    unsigned char k[KEY_BYTES];

    size_t mlen = strlen((char *)text);
//    for(size_t i = 0; i < mlen; i++){
//        printf("%c", text[i]);
//    }
//    printf("\n");
//    print_vector(text, mlen);
//    printf("mlen = %zu\n", mlen);
    unsigned char c[1024]; // Assuming 1024 is sufficient; adjust based on your needs
    unsigned long long clen;
    unsigned char m[N];  // buffer for decrypted message
    unsigned char nsec;                  // not used, can be NULL or any value
    unsigned long long adlen = 16; // length of the associated data
//    printf("=======================INITIALIZATION=======================\n");
    //GENERATING NONCE
    uint8_t npub[16];

    generate_nonce(npub, 16);
//    printf("128-bit NONCE (N):\n");
//    print_vector(npub, 16);
//    printf("\n");

//EMPTY ASSOCIATED DATA
//    unsigned char ad[34] = {};
    unsigned char ad[34] = "Associated Data can be of any size";
    unsigned char modified_ad[34] = "Associated Data can be of any sizf";
    unsigned long long ad_len = strlen((char *)ad);
//    unsigned char k[16] = {0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0xAC, 0xFA, 0x89, 0x00};
//    printf("128-bit KEY (K):\n");
//    print_vector(k, 16);
//    printf("\n");

    //ENCRYPT AEAD WITH MODIFIED ASSOCIATED DATA
    printf("==================AEAD Encryption==================\n");
    unsigned char* ciphertext;

    crypto_aead_encrypt(c, &clen, text, mlen, ad, ad_len, NULL, npub, k);
//    printf("\n");

//    printf("CIPHERTEXT:\n");
//    print_vector(c, clen);
    //DECRYPTION AEAD
    printf("==================AEAD Decryption==================\n");
    size_t result;

    //DECRYPTION WITH SAME ASSOCIATED DATA
    result = crypto_aead_decrypt(m, &mlen, c, clen, ad, ad_len, NULL, npub, k);
//    printf("result = %d\n", result);
    //DECRYPTION WITH MODIFIED ASSOCIATED DATA
//    result = crypto_aead_decrypt(m, &mlen, c, clen, modified_ad, ad_len, NULL, npub, k);
//    result != 0 ? printf("Tag verification is failed.\n") : print_character(m, mlen);
//    printf("result = %zu\n", result);
//    printf("RECOVERED PLAINTEXT:\n");
//    print_vector(m, mlen);
//    print_character(m, mlen);

    return 0;
}