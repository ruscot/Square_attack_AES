#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "oracle.c"

void initBlockToZero(uint8_t block[AES_BLOCK_SIZE]) {
    int index;
    for(index = 0; index < AES_BLOCK_SIZE; index++){
        block[index] = 0x00;
    }
}

void distinguisherRandomPermutationOrAESEncryption() {
    //Key we'll used for the encryption test
    uint8_t k[32] = {   
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 
        0x45, 0x58, 0x0a, 0x16, 0x08, 0xa1, 0x82, 0xa6, 0x9b, 0xd7, 0x15, 0x08, 0x59, 0xbf, 0xaf, 0xbc
    };
    

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
        
        oracleRandomPermutationOrAES(k, currentBlock);

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
    printf("Distinguisher response => ");
    if(isAES == 1){
        printf("We are using 3 rounds AES for encryption !!\n");
    } else {
        printf("We are using a random permutation !!\n");
    }
    free(currentBlock);
    free(cipheredBlock);
    free(initBlock);

}


void distinguisherRightOrWrongKeyGuessed() {
    //Key we'll used for the encryption test
    /*uint8_t k[32] = {   
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 
        0x45, 0x58, 0x0a, 0x16, 0x08, 0xa1, 0x82, 0xa6, 0x9b, 0xd7, 0x15, 0x08, 0x59, 0xbf, 0xaf, 0xbc
    };*/

    uint8_t *currentBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    oracleRandomPermutationOrAES2(currentBlock);
    

    //Compute all Lambda possible 
    /*uint8_t *initBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    initBlockToZero(initBlock);
    uint8_t *currentBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    uint8_t *cipheredBlock = malloc(sizeof(uint8_t) * AES_BLOCK_SIZE);
    initBlockToZero(cipheredBlock);
    
    int lambda;//, index;
    for(lambda = 0; lambda < 256; lambda++) {
        memcpy(currentBlock, initBlock, sizeof(uint8_t)*AES_BLOCK_SIZE+1);
        currentBlock[0] = lambda;
        
        oracleRandomPermutationOrAES2(currentBlock);

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
    printf("Distinguisher response => ");
    if(isAES == 1){
        printf("We are using 3 rounds AES for encryption !!\n");
    } else {
        printf("We are using a random permutation !!\n");
    }
    free(currentBlock);
    free(cipheredBlock);
    free(initBlock);*/

}