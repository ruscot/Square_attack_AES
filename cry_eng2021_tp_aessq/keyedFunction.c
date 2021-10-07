#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "aes-128_enc.h"
#include "aes-128_enc.c"

void keyedFunction(uint8_t k1[AES_BLOCK_SIZE], uint8_t k2[AES_BLOCK_SIZE], uint8_t block[AES_BLOCK_SIZE]) {
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

    free(next_key);
}

int main(){
    printf("This program implement the keyed function F defined as below :\n"
        "Given k1, k2 and x, F(k1||k2, x) = E(k1, x) xor E(k2, x)\n"
        "For our case E is equal to round of AES with ki the key and x the message to cipher\n"
        "To be non trivial we need k1 != k2 because if k1 = k2 => E(k1, x) = E(k2, x) so F(k1||k2, x) = 0\n");
    
    uint8_t *k1 = malloc(sizeof(uint8_t) * 16);
    
    k1[0] = 0x2b;
    k1[1] = 0x7e;
    k1[2] = 0x15;
    k1[3] = 0x16;
    k1[4] = 0x28;
    k1[5] = 0xae;
    k1[6] = 0xd2;
    k1[7] = 0xa6;
    k1[8] = 0xab;
    k1[9] = 0xf7;
    k1[10] = 0x15;
    k1[11] = 0x88;
    k1[12] = 0x09;
    k1[13] = 0xcf;
    k1[14] = 0x4f;
    k1[15] = 0x3c;

    uint8_t *k2 = malloc(sizeof(uint8_t) * 16);
    
    k2[0] = 0x45;
    k2[1] = 0x58;
    k2[2] = 0x0a;
    k2[3] = 0x16;
    k2[4] = 0x08;
    k2[5] = 0xa1;
    k2[6] = 0x82;
    k2[7] = 0xa6;
    k2[8] = 0x9b;
    k2[9] = 0xd7;
    k2[10] = 0x15;
    k2[11] = 0x08;
    k2[12] = 0x59;
    k2[13] = 0xbf;
    k2[14] = 0xaf;
    k2[15] = 0xbc;
    
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

    for(int i = 0; i < 16; i++){
        printf(" %d ",x[i]);
    }
    printf("$\n");

    keyedFunction(k1, k2, x);

    for(int i = 0; i < 16; i++){
        printf(" %d ",x[i]);
    }
    printf("$\n");

    free(k1);
    free(k2);
    free(x);
    return 0;
}