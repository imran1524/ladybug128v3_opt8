#include "api.h"
#include "aead.h"
#include "permutations.h"
#include "utils.h"

//ENCRYPTION
size_t crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                           const unsigned char* m, unsigned long long mlen,
                           const unsigned char* ad, unsigned long long adlen,
                           const unsigned char* nsec,
                           const unsigned char* npub,
                           const unsigned char* k){
    (void)nsec;
    //SET THE POINTER OF CIPHERTEXT BY ADDING MESSAGE LENGTH AND ASSOCIATED DATA LENGTH
    *clen = mlen + AD_BYTES;

    //LOAD KEY AND NONCE
    //key, k is 128-bits that is divided into two 64-bit keys, public nonce, npub is 128-bits
    const uint64_t K0 = LOAD_BYTES(k, 8);
    const uint64_t K1 = LOAD_BYTES(k + 8, 8);
    const uint64_t N0 = LOAD_BYTES(npub, 8);
    const uint64_t N1 = LOAD_BYTES(npub + 8, 8);

    //INITIALIZATION:

    state_t s;
    printf("IV: %0llx\n", AEAD_128_IV);
    s.x[0] = AEAD_128_IV;
    s.x[1] = K0;
    s.x[2] = K1;
    s.x[3] = N0;
    s.x[4] = N1;
//    printf("STATE AFTER INITALIZATION\n");
//    print_state(&s);
//    printf("\n");

    //APPLY FORWARD PERMUTATION LAYER
    FP1(&s);
//    printf("STATE AFTER FIRST PERMUTATION\n");
//    print_state(&s);
//    printf("\n");
    //XORING KEY 0*||K
    s.x[3] ^= K0;
    s.x[4] ^= K1;

//    printf("State after initialization WITH KEY\n");
//    print_state(&s);
//    printf("\n");
//    END OF INITIALIZATIO DOMAIN

//PROCESSING ASSOCIATED DATA
    printf("=======================PROCESSING ASSOCIATED DATA (AD) =======================\n");
    if (adlen){
        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= RATE){
            printf("adlen = %d\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);
//            printf("ABSORB ASSOCIATED DATA:\n");
//            print_state(&s);
//            printf("\n");

            //APPLY A FORWARD PERMUTATION OPERATION
            FP1(&s);
//            printf("STATE AFTER PROCESSING EACH ASSOCIATED BLOCK\n");
//            print_state(&s);
//            printf("\n");
//            printf("adlen_final = %d\n", adlen);
            ad += RATE;
            adlen -= RATE;
        }

        //FINAL ASSOCIATED DATA BLOCK
        s.x[0] ^= LOAD_BYTES(ad, adlen);
//        print_bitstring(PAD(adlen), 8);
        s.x[0] ^= PAD(adlen);
//        printf("PADDED ASSOCIATED DATA\n");
//        print_state(&s);
//        printf("\n");

        //APPLY FORWARD PERMUTATION
        FP1(&s);
    }

    //DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND PLAINTEXT
    s.x[4] ^= 1;
//    printf("STATE AFTER DOMAIN SEPARATION\n");
//    print_state(&s);
//    printf("\n");
    //END OF ASSOCIATED DOMAIN

//PROCESSING CIPHERTEXT
    //FULL PLAINTEXT BLOCKS
    printf("=======================PLAINTEXT BLOCK=======================\n");
    size_t block = 0;
    while (mlen >= RATE){
        //LOADING 8 BYTES FROM THE MESSAGE AND XOR WITH THE FIRST BLOCK OF THE STATE
        printf("Message block: %d\n", block + 1);
        printf("mlen = %d\n", mlen);
        printf("\n");

        printf("Message Block = %d\n", block + 1);
        print_vector(m, 8);
        printf("\n");

        s.x[0] ^= LOAD_BYTES(m, 8);
        STORE_BYTES(c, s.x[0], 8);
        printf("Ciphertext block: %d\n", block + 1);
        print_vector(c, 8);
        printf("Address of c variable = %p\n", (void*)&c);
        printf("Current address pointed by c = %p\n", (void*)c);
        printf("\n");

//        printf("STATE AFTER ABSORBING PLAINTEXT BEFORE APPLYING PERMUTATION\n");
//        print_state(&s);
//        printf("\n");

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

    //FINAL PLAINTEXT BLOCK
    printf("Final message size = %d\n", mlen);
    s.x[0] ^= LOAD_BYTES(m, mlen);
    printf("Final message block\n");
    print_vector(m, 8);
    printf("\n");
    STORE_BYTES(c, s.x[0], mlen);
    printf("Final ciphetext:\n");
    print_vector(c, 8);
    s.x[0] ^= PAD(mlen);
    c += mlen;
//    printf("\n");
//    printf("Padded plaintext\n");
//    print_state(&s);
//    printf("\n");
//    END OF PLAINtext

    //FINALIZATION DOMAIN
    //START OF FINALIZATION
    printf("=======================FINALIZATION=======================\n");
    s.x[1] ^= K0;
    s.x[2] ^= K1;
//    printf("STATE AFTER FIRST KEY XOR\n");
//    print_state(&s);
//    printf("\n");
    //APPLY FORWARD PERMUTATION
    FP1(&s);

    //XORING K||0*
    s.x[3] ^= K0;
    s.x[4] ^= K1;
//    printf("STATE AFTER second KEY XOR\n");
//    print_state(&s);
//    printf("\n");

    //SET TAG
    //128-bit tag is the last two block of the most updated state and stored at the end of ciohertext
    printf("Tag\n");
    printf("Current address pointed by c = %p\n", (void*)c);
    STORE_BYTES(c, s.x[3], 8);
    STORE_BYTES(c + 8, s.x[4], 8);

    return 0;
}

//======================================================================================================================
//DECRYPTION AEAD
size_t crypto_aead_decrypt(unsigned char* m, unsigned long long *mlen,
                           const unsigned char* c, unsigned long long clen,
                           const unsigned char* ad, unsigned long long adlen,
                           unsigned char* nsec,
                           const unsigned char* npub,
                           const unsigned char* k){

    (void)nsec;
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
//    printf("Initialization first key xor\n");
//    print_state(&s);
//    printf("\n");

    //APPLY INVERSE PERMUTATION
    IP1(&s);
    s.x[3] ^= K0;
    s.x[4] ^= K1;
    printf("Inilialization second key xor\n");
    //END OF INILIALIZATION DOMAIN

//START OF ASSOCIATED DATA DOMAIN
    if(adlen){
        //FULL ASSOCIATED DATA BLOCKS
        while(adlen >= RATE){
            printf("adlen = %d\n", adlen);
            s.x[0] ^= LOAD_BYTES(ad, 8);
//            printf("Absorb associated data\n");
//            print_state(&s);
//            printf("\n");
            //APPLY INVERSE PERMUTATION
            IP1(&s);
            ad += RATE;
            adlen -= RATE;
        }
        //FINAL ASSOCIATED DATA BLOCK
        printf("adlen = %d\n", adlen);
        s.x[0] ^= LOAD_BYTES(ad, adlen);
        s.x[0] ^= PAD(adlen);

//        printf("PADDED ASSOCIATED DATA\n");
//        print_state(&s);
//        printf("\n");

        //APPLY INVERSE PERMUTATION
        IP1(&s);
    }
//END OF ASSOCIATED DATA DOMAIN
//DOMAIN SEPARATION BETWEEN ASSOCIATED DATA AND CIPHERTEXT
    s.x[4] ^= 1;
//    printf("Domain separation\n");
//    print_state(&s);
//    printf("\n");

//START OF CIPHERTEXT DOMAIN
    //FULL CIPHERTWEXT BLOCK
    //ADJUSTING CIPHERTEXT BY SUBTRACTING THE TAG LENGTH
    clen -= AD_BYTES;
    size_t block = 0;
    while(clen >= RATE){
        uint64_t c0 = LOAD_BYTES(c, 8);
        printf("Ciphetext block = %d\n", block + 1);
        print_vector(c, 8);
        printf("\n");
        STORE_BYTES(m, s.x[0] ^ c0, 8);
        printf("clen = %d\n", clen);
        printf("Message block = %d\n", block + 1);
        print_vector(m, 8);
        printf("\n");
        s.x[0] = c0;
//        printf("Insert ciphertext:\n");
//        print_state(&s);
//        printf("\n");

        //INVERSE PERMUTATION
        IP1(&s);
        m += RATE;
        c += RATE;
        clen -= RATE;
        block++;
    }

    //FINAL CIPHERTEXT BLOCK
    uint64_t c0 = LOAD_BYTES(c, clen);
    printf("Final ciphertext_block\n");
    print_vector(c, 8);
    printf("\n");
    STORE_BYTES(m, s.x[0] ^c0, clen);
    printf("Final message_block\n");
    print_vector(m, 8);
    printf("\n");
    s.x[0] = CLEAR_BYTES(s.x[0], clen);

//    print_vector(s.x[0], 8);
    s.x[0] |= c0;
    s.x[0] ^= PAD(clen);
    c += clen;
//    printf("Print the padded ciphertext\n");
//    print_state(&s);
//    printf("\n");

    //FINALIZE
    s.x[1] ^= K0;
    s.x[2] ^= K1;
//    printf("Final first key XOR\n");
//    print_state(&s);
//    printf("\n");

    //APPLY INVERSE PERMUTATION
    IP1(&s);
    s.x[3] ^= K0;
    s.x[4] ^= K1;
//    printf("Final second key XOR\n");
//    print_state(&s);
//    printf("\n");

    //SET TAG
    uint8_t t[16];
    STORE_BYTES(t, s.x[3], 8);
    STORE_BYTES(t + 8, s.x[4], 8);

    //VERIFY TAG
    size_t i;
    size_t result = 0;
    for(i = 0; i < AD_BYTES; i++){
        result |= c[i] ^ t[i];
    }

    //VERIFY TAG (THIS OPERATION MUST BE CONSTANT TIME)
    result = (((result - 1) >> 8) & 1) - 1;
    return result;
}

