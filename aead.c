#include "api.h"
#include "aead.h"
#include "permutations.h"
#include "utils.h"

//ENCRYPTION
size_t crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                           const unsigned char* m, size_t mlen,
                           const unsigned char* ad, size_t adlen,
                           const unsigned char* nsec,
                           const unsigned char* npub,
                           const unsigned char* k){
    (void)nsec;
    //SET THE POINTER OF CIPHERTEXT BY ADDING MESSAGE LENGTH AND ASSOCIATED DATA LENGTH
    *clen = mlen + AD_BYTES;

//    printf("NONCE USED IN AEAD ENCRYPTION\n");
//    print_vector(npub, 16);
//    printf("\n");
//
//    printf("ASSOCIATED DATA USED IN AEAD ENCRYPTION\n");
//    print_vector(ad, adlen);
//    printf("\n");
//
//    printf("KEY USED IN AEAD ENCRYPTION\n");
//    print_vector(k, 16);
//    printf("\n");

    //LOAD KEY AND NONCE
    //key, k is 128-bits that is divided into two 64-bit keys, public nonce, npub is 128-bits
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

    //INITIALIZATION:
    state_t s;
//    printf("IV: %0llx\n", AEAD_128_IV);
    s.x[0] = AEAD_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;

//    printf("1. AEAD ENCRYPTION INITIALIZATION: STATE BEFORE FORWARD TRANSFORM\n");
//    print_state(&s);
//    printf("\n");

    //APPLY FORWARD PERMUTATION LAYER
    FP1(&s);
//    printf("2. AEAD ENCRYPTION INITIALIZATION: STATE AFTER FORWARD TRANSFORM\n");
//    print_state(&s);
//    printf("\n");

    //XORING KEY 0*||K
    s.x[3] ^= K0;
    s.x[4] ^= K1;
//    printf("3. AEAD ENCRYPTION INITIALIZATION: STATE AFTER APPLYING KEY\n");
//    print_state(&s);
//    printf("\n");
// END OF INITIALIZATIO DOMAIN

//PROCESSING ASSOCIATED DATA
//    printf("=======================PROCESSING ASSOCIATED DATA (AD) =======================\n");
    if (adlen){
        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= RATE){
//            printf("adlen in encryption = %zu\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);
//            printf("4. AEAD ENCRYPTION AD: STATE PROCESSING FULL AD BEFORE APPLYING FORWARD PERMUTATION\n");
//            print_state(&s);
//            printf("\n");

            //APPLY A FORWARD PERMUTATION OPERATION
            FP1(&s);
//            printf("5. AEAD ENCRYPTION AD: STATE PROCESSING FULL AD AFTER APPLYING FORWARD PERMUTATION \n");
//            print_state(&s);
//            printf("\n");

//            printf("adlen_final = %d\n", adlen);
            ad += RATE;
            adlen -= RATE;
        }

        //FINAL ASSOCIATED DATA BLOCK
        s.x[0] ^= LOAD_BYTES(ad, adlen);
//        print_bitstring(PAD(adlen), 8);

//        printf("6. AEAD ENCRYPTION AD: FINAL STATE BEFORE PADDING\n");
//        print_state(&s);
//        printf("\n");

        s.x[0] ^= PAD(adlen);
//        printf("7. AEAD ENCRYPTION AD: FINAL STATE AFTER PADDING\n");
//        print_state(&s);
//        printf("\n");

        //APPLY FORWARD PERMUTATION
        FP1(&s);
//        printf("8. AEAD ENCRYPTION AD: STATE AFTER FORWARD PERMUTATION\n");
//        print_state(&s);
//        printf("\n");
    }

    //DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND PLAINTEXT
        s.x[4] ^= 1;
    //printf("9. AEAD ENCRYPTION AD: STATE AFTER DOMAIN SEPARATION BETWEEN ASSOCIATED DATA PLAINTEXT\n");
    //print_state(&s);
    //printf("\n");

    //END OF ASSOCIATED DOMAIN
    //======================== FINAL PLAINTEXT BLOCK PROCESSING =======================================
    size_t block = 0;
    uint64_t test  = 0;
    while (mlen >= RATE){
        //LOADING 8 BYTES FROM THE MESSAGE AND XOR WITH THE FIRST BLOCK OF THE STATE
//        printf("Message block: %d\n", block + 1);
//        printf("mlen = %d\n", mlen);
//        printf("\n");

//        printf("Message Block = %d\n", block + 1);
//        print_vector(m, 8);
//        printf("\n");

        s.x[0] ^= LOAD_BYTES(m, 8);
        STORE_BYTES(c, s.x[0], 8);
//        printf("Ciphertext block: %d\n", block + 1);
//        print_vector(c, 8);
//        printf("Address of c variable = %p\n", (void*)&c);
//        printf("Current address pointed by c = %p\n", (void*)c);
//        printf("\n");
//        printf("STATE AFTER ABSORBING PLAINTEXT BEFORE APPLYING PERMUTATION\n");
//        print_state(&s);
        printf("\n");

        //APPLY FORWARD PERMUTATION
        FP1(&s);
//        printf("STATE AFTER ABSORBING PLAINTEXT AFTER APPLYING PERMUTATION\n");
//        print_state(&s);
//        printf("\n");
        m += RATE;
        c += RATE;
        mlen -= RATE;
        block++;
    }

    //======================== FINAL PLAINTEXT BLOCK PROCESSING =======================================
//    printf("Final message size = %zu\n", mlen);
//    printf("FINAL MESSAGE BLOCK:\n");
//    print_vector(m, 8);
//    printf("\n");

//    printf("STATE BEFORE XORING WITH THE LOADED PLAINTEXT\n");
//    print_state(&s);
//    printf("\n");

//    printf("STATE BEFORE XORING WITH THE LOADED PLAINTEXT IN BLOCK\n");
//    print_data_byte(&s);
//    printf("\n");

//    printf("mlen in the final round = %zu\n", mlen);
    //LOADING FINAL MESSAGE BLOCK AND XOR WITH THE STATE
    s.x[0] ^= LOAD_BYTES(m, mlen); //mlen = 3

    //Operation #1
    //LOAD_BYTES(m, mlen) is function which returns a 64-bit number by
    //loading mlen bytes from m
//    s.x[0] = s.x[0] ^ LOAD_BYTES(m, mlen);

//    printf("STATE AFTER PROCESSING LOADING MESSAGE FINAL BLOCK TO STATE WITHOUT PADDING\n");
//    print_data_byte(&s);

//    printf("STATE AFTER XORING WITH LOADED PLAINTEXT\n");
//    print_state(&s);
//    printf("\n");

//    printf("STATE AFTER XORING WITH THE LOADED PLAINTEXT IN BLOCK\n");
//    print_data_byte(&s);
//    printf("\n");]

    //PRODUCING FINAL CIPHERTEXT BLOCK FROM FINAL PLAINTEXT BLOCK
    STORE_BYTES(c, s.x[0], mlen);
//    printf("FINAL CIPHERTEXT BLOCK WITHOUT PADDING:\n");
//    print_vector(c, 8);
//    printf("\n");

//    printf("------------------STATE BEFORE ADDING PADDING----------------------\n");
//    print_data_byte(&s);
//    printf("\n");

//    printf("ENCRYPTION: STATE BEFORE PADDING \n");
//    print_state(&s);
//    printf("\n");

//    printf("ENCRYPTION: STATE BEFORE PADDING IN BLOCK\n");
//    print_data_byte(&s);
//    printf("\n");

    //APPLYING PADDING TO THE STATE AFTER PROCESSING FINAL CIPHERTEXT BLOCK
    s.x[0] ^= PAD(mlen);
//    printf("ENCRYPTION: STATE AFTER PADDING \n");
//    print_state(&s);
//    printf("\n");

//    printf("ENCRYPTION: STATE AFTER PADDING IN BLOCK\n");
//    print_data_byte(&s);
//    printf("\n");

//    printf("------------------STATE AFTER ADDING PADDING-----------------------\n");
//    print_data_byte(&s);
    c += mlen;

    //FINALIZATION DOMAIN
    //START OF FINALIZATION
//    printf("=======================FINALIZATION=======================\n");
    s.x[1] ^= K0;
    s.x[2] ^= K1;

//    printf("STATE BEFORE FIRST KEY XOR\n");
//    print_state(&s);
//    printf("\n");

    //APPLY FORWARD PERMUTATION
    FP1(&s);

//    printf("STATE AFTER FIRST KEY XOR\n");
//    print_state(&s);
//    printf("\n");

    //XORING K||0*
    s.x[3] ^= K0;
    s.x[4] ^= K1;

//    printf("STATE AFTER APPLYING KEY\n");
//    print_state(&s);
//    printf("\n");

    //SET TAG
    //128-bit tag is the last two block of the most updated state and stored at the end of ciphertext
//    printf("Tag\n");
//    printf("Current address pointed by c = %p\n", (void*)c);
    STORE_BYTES(c, s.x[3], 8); //LOADING FIRST PART OF 64-BIT TAG INTO THE FOURTH BLOCK OF THE STATE
    STORE_BYTES(c + 8, s.x[4], 8); //LOADING SECOND PART OF THE 64-BIT TAGE INTO THE FIFTH BLOCK OF THE STATE
    return 0;
}

//======================================================================================================================
//DECRYPTION AEAD
size_t crypto_aead_decrypt(unsigned char* m, unsigned long long *mlen,
                           const unsigned char* c, size_t clen,
                           const unsigned char* ad, size_t adlen,
                           const unsigned char* nsec,
                           const unsigned char* npub,
                           const unsigned char* k){
    (void)nsec;
//    printf("NONCE USED IN AEAD DECRYPTION\n");
//    print_vector(npub, 16);b
//    printf("\n");

//    printf("ASSOCIATED DATA USED IN AEAD DECRYPTION\n");
//    print_vector(ad, adlen);
//    printf("\n");

//    printf("KEY USED IN AEAD DECRYPTION\n");
//    print_vector(k, 16);
//    printf("\n");

    if(clen < AD_BYTES){
        return -1;
    }

    //SET PLAINTEXT SIZE
    *mlen = clen - AD_BYTES;

    //LOAD KEY AND NONCE
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

//START OF INITIALIZATION DOMAIN
    state_t s;
    s.x[0] = AEAD_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;
//    printf("1. AEAD DECRYPTION INITIALIZATION: STATE BEFORE FIRST FORWARD TRANSFORM\n");
//    print_state(&s);
//    printf("\n");

    //APPLY FORWARD PERMUTATION
    FP1(&s);
//    printf("2. AEAD DECRYPTION INITIALIZATION: STATE AFTER FORWARD TRANSFORM\n");
//    print_state(&s);
//    printf("\n");

    s.x[3] ^= K0;
    s.x[4] ^= K1;
//    printf("3. AEAD DECRYPTION INITIALIZATION: STATE AFTER APPLYING KEY\n");
//    print_state(&s);
//    printf("\n");

//END OF INILIALIZATION DOMAIN
//START OF ASSOCIATED DATA DOMAIN
    if(adlen){

        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= RATE){
//            printf("adlen in decryption = %d\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);
//            printf("4. AEAD DECRYPTION AD: STATE BEFORE PROCESSING FULL ASSOCIATED DATA\n");
//            print_state(&s);
//            printf("\n");

            //APPLY FORWARD PERMUTATION
            FP1(&s);
//            printf("5. AEAD DECRYPTION AD: STATE AFTER APPLYING FORWARD PERMUTATION \n");
//            print_state(&s);
//            printf("\n");

            ad += RATE;
            adlen -= RATE;
        }

        //FINAL ASSOCIATED DATA BLOCK
//        printf("adlen = %d\n", adlen);
        s.x[0] ^= LOAD_BYTES(ad, adlen);

//        printf("6. AEAD DECRYPTION AD: STATE AFTER PADDING\n");
//        print_state(&s);
//        printf("\n");

//        printf("DECRYPTION: STATE BEFORE PADDING IN BLOCKS\n");
//        print_data_byte(&s);
//        printf("\n");

        s.x[0] ^= PAD(adlen);
//        printf("7. AEAD DECRYPTION AD: STATE AFTER PROCESSING PADDED AD BEFORE FORWARD PERMUTATION\n");
//        print_state(&s);
//        printf("\n");

        //APPLY FORWARD PERMUTATION
        FP1(&s);
//        printf("8. AEAD DECRYPTION AD: STATE AFTER PROCESSING PADDED AD AFTER FORWARD PERMUTATION\n");
//        print_state(&s);
//        printf("\n");
    }

//END OF ASSOCIATED DATA DOMAIN
//DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND CIPHERTEXT
    s.x[4] ^= 1;
//    printf("9. AEAD DECRYPTION AD: STATE AFTER DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND PLAINTEXT\n");
//    print_state(&s);
//    printf("\n");

//START OF CIPHERTEXT DOMAIN
    //FULL CIPHERTWEXT BLOCK
    //ADJUSTING CIPHERTEXT BY SUBTRACTING THE TAG LENGTH
    clen -= AD_BYTES;
    size_t block = 0;
    while(clen >= RATE){
        uint64_t c0 = LOAD_BYTES(c, 8);
//        printf("Ciphetext block = %d\n", block + 1);
//        print_vector(c, 8);
//        printf("\n");

        STORE_BYTES(m, s.x[0] ^ c0, 8);
//        printf("Message block = %d\n", block + 1);
//        print_vector(m, 8);
//        printf("\n");

        s.x[0] = c0;
//        printf("Insert ciphertext:\n");
//        print_state(&s);
//        printf("\n");

        //INVERSE PERMUTATION
        FP1(&s);
        m += RATE;
        c += RATE;
        clen -= RATE;
        block++;
    }

    //FINAL CIPHERTEXT BLOCK
//    printf("clen = %zu\n", clen);
    uint64_t c0 = LOAD_BYTES(c, clen);
//    printf("Final ciphertext_block\n");
//    print_vector(c, 8);
//    printf("\n");

    STORE_BYTES(m, s.x[0] ^c0, clen);
//    printf("Final message_block\n");
//    print_vector(m, 8);
//    printf("\n");

//    printf("DECRYPTION: BEFORE CLEAR STATE\n");
//    print_state(&s);
//    printf("\n");

    s.x[0] = CLEAR_BYTES(s.x[0], clen);
//    printf("DECRYPTION: AFTER CLEAR STATE\n");
//    print_state(&s);
//    printf("\n");
    s.x[0] |= c0;

//    printf("DECRYPTION: STATE \n");
//    print_state(&s);
//    printf("\n");

//    printf("DECRYPTION: STATE BEFORE PADDING IN BLOCKS\n");
//    print_data_byte(&s);
//    printf("\n");

    s.x[0] ^= PAD(clen);
//    printf("DECRYPTION: STATE AFTER PADDING\n");
//    print_state(&s);
//    printf("\n");

//    printf("DECRYPTION: STATE AFTER PADDING IN BLOCKS\n");
//    print_data_byte(&s);
//    printf("\n");
    c += clen;

    //FINALIZE
    s.x[1] ^= K0;
    s.x[2] ^= K1;
//    printf("Final first key XOR\n");
//    print_state(&s);
//    printf("\n");

    //APPLY INVERSE PERMUTATION
    FP1(&s);
    s.x[3] ^= K0;
    s.x[4] ^= K1;
//    printf("Final second key XOR\n");
//    print_state(&s);
//    printf("\n");

    //SET TAG
    uint8_t t[16];
    STORE_BYTES(t, s.x[3], 8); //LOAD FIRST PART OF 64-BIT TAG ON FORTH PART OF THE KEY
    STORE_BYTES(t + 8, s.x[4], 8); //LOAD SECOND PART OF 64-BIT TAG ON FIFTH PART OF THE KEY

    //VERIFY TAG BY COMPARING THE GENERATED TAG WITH THE ORIGINAL TAG
    size_t i;
    size_t result = 0;
    for(i = 0; i < AD_BYTES; i++){
        result |= c[i] ^ t[i];
//        printf("Result = %zu\n", result);
    }

    //VERIFY TAG (THIS OPERATION MUST BE CONSTANT TIME)
    result = (((result - 1) >> 8) & 1) - 1;

    return  result;
}

void print_binary(unsigned char* binary, size_t binary_len) {
    for(size_t i = 0; i < binary_len; i++) {
        // Print each byte in binary
        for(int bit = 7; bit >= 0; bit--) {
            putchar((binary[i] >> bit) & 1 ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}
