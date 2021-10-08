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

    aes128_enc(block, k1, 3, 1);

    //Third compute the 3 AES round for k2 on block
    aes128_enc(saveBlock, k2, 3, 1);

    //Last do xor for the two ciphered message
    for(int lengthKey = 0; lengthKey < AES_BLOCK_SIZE; lengthKey++){
        block[lengthKey] ^= saveBlock[lengthKey];
    }

    free(k1);
    free(k2);
    free(saveBlock);
    //free(next_key);
}

/**
 * Function taken on the internet 
*/
void randomPermutation(uint8_t block[AES_BLOCK_SIZE]) {
    for (int i = AES_BLOCK_SIZE-1; i >= 0; --i){
        //generate a random number between [0, AES_BLOCK_SIZE-1]
        int j = rand() % (i+1);

        //swap the last element with element at random index
        int temp = block[i];
        block[i] = block[j];
        block[j] = temp;
    }
}


void oracleRandomPermutationOrAES(uint8_t k[AES_BLOCK_SIZE * 2], uint8_t block[AES_BLOCK_SIZE]) {
    static int chooseEncryption = -1;
    
    if(chooseEncryption == -1){
        srand(time(NULL));
        chooseEncryption = rand() % 2;
        printf("\nOracle encryption choosed %d => ", chooseEncryption);
        if(chooseEncryption == 1){
            printf("We are using 3 rounds AES for encryption\n");
        } else {
            printf("We are using a random permutation\n");
        }
    }

    if(chooseEncryption == 1){
        keyedFunction(k, block);
    } else {
        randomPermutation(block);
    }

}

/**
 * Give the 3 round 1/2 AES encryption
*/
void oracleSquareAttack(uint8_t block[AES_BLOCK_SIZE]) {
    static uint8_t *k = NULL;
    
    if(k == NULL){
        k = malloc(sizeof(uint8_t) * AES_128_KEY_SIZE);
        FILE *fp;
        char data[AES_128_KEY_SIZE*8];
        fp = fopen("/dev/urandom", "r");
        fread(&data, 1, AES_128_KEY_SIZE * 8, fp);
        fclose(fp);

        for(int i = 0; i < AES_128_KEY_SIZE; i++) {
            k[i] = (uint8_t) data[i];
        }
    }

    aes128_enc(k, block, 4, 0);
}