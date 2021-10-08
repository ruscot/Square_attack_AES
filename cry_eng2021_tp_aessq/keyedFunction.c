#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "aes-128_enc.h"
#include "aes-128_enc.c"

void keyedFunction(uint8_t k[AES_BLOCK_SIZE * 2], uint8_t block[AES_BLOCK_SIZE]) {
    uint8_t *k1 = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    uint8_t *k2 = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    memcpy(k1, k, sizeof(uint8_t)*AES_BLOCK_SIZE+1);
    memcpy(k2, k + sizeof(uint8_t)*AES_BLOCK_SIZE, sizeof(uint8_t)*AES_BLOCK_SIZE+1);
    
    //First save the clear message for the ciphering with k2
    uint8_t *saveBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    memcpy(saveBlock, block, sizeof(uint8_t)*AES_BLOCK_SIZE+1);

    int round;
    uint8_t *next_key = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    //Second compute the 3 AES round for k1 on block
    for(round = 0; round < 3; round++){
        aes_round(block, k1, round==3 ? 16 : 0);
        next_aes128_round_key(k1, next_key, round);
        for(int lengthKey = 0; lengthKey < AES_BLOCK_SIZE; lengthKey++){
            k1[lengthKey] = next_key[lengthKey];
            next_key[lengthKey] = 0;
        }
    }

    //Third compute the 3 AES round for k2 on block
    for(round = 0; round < 3; round++){
        aes_round(saveBlock, k2, round==3 ? 16 : 0);
        next_aes128_round_key(k2, next_key, round);
        for(int lengthKey = 0; lengthKey < AES_BLOCK_SIZE; lengthKey++){
            k2[lengthKey] = next_key[lengthKey];
            next_key[lengthKey] = 0;
        }
    }    

    //Last do xor for the two ciphered message
    for(int lengthKey = 0; lengthKey < AES_BLOCK_SIZE; lengthKey++){
        block[lengthKey] ^= saveBlock[lengthKey];
    }

    free(k1);
    free(k2);
    free(saveBlock);
    free(next_key);
}

/**
 * Function take on internet 
*/
void randomPermutation(uint8_t k[AES_BLOCK_SIZE * 2], uint8_t block[AES_BLOCK_SIZE]) {
    for (int i = AES_BLOCK_SIZE-1; i >= 0; --i){
        //generate a random number between [0, AES_BLOCK_SIZE-1]
        int j = rand() % (i+1);

        //swap the last element with element at random index
        int temp = block[i];
        block[i] = block[j];
        block[j] = temp;
    }
}

void initBlockToZero(uint8_t block[AES_BLOCK_SIZE]) {
    int index;
    for(index = 0; index < AES_BLOCK_SIZE; index++){
        block[index] = 0x00;
    }
}

void distinguisher() {
    //Key we'll used for the encryption test
    uint8_t k[32] = {   
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 
        0x45, 0x58, 0x0a, 0x16, 0x08, 0xa1, 0x82, 0xa6, 0x9b, 0xd7, 0x15, 0x08, 0x59, 0xbf, 0xaf, 0xbc
    };
    srand(time(NULL));
    int functionUsed = rand() % 2;

    //Compute all Lambda possible 
    uint8_t *initBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    initBlockToZero(initBlock);
    uint8_t *currentBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    uint8_t *cipheredBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    initBlockToZero(cipheredBlock);
    

    int lambda;//, index;
    for(lambda = 0; lambda < 256; lambda++) {
        memcpy(currentBlock, initBlock, sizeof(uint8_t)*AES_BLOCK_SIZE+1);
        currentBlock[0] = lambda;
        if(functionUsed == 1){
            keyedFunction(k, currentBlock);
        } else {
            randomPermutation(k, currentBlock);
        }

        //Xor new block with last block 
        (*cipheredBlock) ^= (*currentBlock);
    }

    printf("\nIf all bytes of the ciphered block are equal to 0\n"
        "It means we are using 3 rounds of AES for ciphering.\n\n");
    int index, isAES = 1;
    for(index = 0; index < AES_BLOCK_SIZE; index++){
        if(cipheredBlock[index] != 0){
            isAES = 0;
        }
    }
    printf("Distinguisher response : \n");
    if(isAES == 1){
        printf("We are using 3 rounds AES for encryption !!\n");
    } else {
        printf("We are using a random permutation !!\n");
    }
    printf("\nResponse : %d\n", functionUsed);
    if(functionUsed == 1){
        printf("We are using 3 rounds AES for encryption !!\n");
    } else {
        printf("We are using a random permutation !!\n");
    }
    free(currentBlock);
    free(cipheredBlock);
    free(initBlock);

}

int main(){
    printf("\nThis program implement the keyed function F defined as below :\n"
        "Given k1, k2 and x, F(k1||k2, x) = E(k1, x) xor E(k2, x)\n"
        "For our case E is equal to round of AES with ki the key and x the message to cipher\n"
        "To be non trivial we need k1 != k2 because if k1 = k2 => E(k1, x) = E(k2, x) so F(k1||k2, x) = 0\n");
    
    uint8_t *k = malloc(sizeof(uint8_t) * 32);
    
    k[ 0] = 0x2b;
    k[ 1] = 0x7e;
    k[ 2] = 0x15;
    k[ 3] = 0x16;
    k[ 4] = 0x28;
    k[ 5] = 0xae;
    k[ 6] = 0xd2;
    k[ 7] = 0xa6;
    k[ 8] = 0xab;
    k[ 9] = 0xf7;
    k[10] = 0x15;
    k[11] = 0x88;
    k[12] = 0x09;
    k[13] = 0xcf;
    k[14] = 0x4f;
    k[15] = 0x3c;
    k[16] = 0x45;
    k[17] = 0x58;
    k[18] = 0x0a;
    k[19] = 0x16;
    k[20] = 0x08;
    k[21] = 0xa1;
    k[22] = 0x82;
    k[23] = 0xa6;
    k[24] = 0x9b;
    k[25] = 0xd7;
    k[26] = 0x15;
    k[27] = 0x08;
    k[28] = 0x59;
    k[29] = 0xbf;
    k[30] = 0xaf;
    k[31] = 0xbc;
    
    uint8_t *x = malloc(sizeof(uint8_t) * 16);
    
    x[0] = 0x89;
    x[1] = 0x08;
    x[2] = 0xba;
    x[3] = 0x96;
    x[4] = 0xa8;
    x[5] = 0xa5;
    x[6] = 0x86;
    x[7] = 0xb6;
    x[8] = 0x00;
    x[9] = 0xab;
    x[10] = 0xc5;
    x[11] = 0xf8;
    x[12] = 0x5b;
    x[13] = 0xb5;
    x[14] = 0x21;
    x[15] = 0x8c;
    distinguisher();
    /*for(int i = 0; i < 16; i++){
        printf(" %d ",x[i]);
    }
    printf("$\n");

    keyedFunction(k, x);

    for(int i = 0; i < 16; i++){
        printf(" %d ",x[i]);
    }
    printf("$\n");
    */
    free(k);
    free(x);
    return 0;
}