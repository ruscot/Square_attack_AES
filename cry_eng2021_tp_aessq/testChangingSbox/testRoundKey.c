#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "aes-128_enc.h"
#include "aes-128_enc.c"

int main(){
    printf("This test check if our function prev_aes128_round_key works\nFor each round we'll calculate the next key = k2 from the previous one = k\nand test if prev_aes128_round_key(k2) = k if it's not the case we'll print doesn't work \n");
    uint8_t *prev_key = malloc(sizeof(uint8_t) * 16);
    
    prev_key[0] = 0x2b;
    prev_key[1] = 0x7e;
    prev_key[2] = 0x15;
    prev_key[3] = 0x16;
    prev_key[4] = 0x28;
    prev_key[5] = 0xae;
    prev_key[6] = 0xd2;
    prev_key[7] = 0xa6;
    prev_key[8] = 0xab;
    prev_key[9] = 0xf7;
    prev_key[10] = 0x15;
    prev_key[11] = 0x88;
    prev_key[12] = 0x09;
    prev_key[13] = 0xcf;
    prev_key[14] = 0x4f;
    prev_key[15] = 0x3c;

    uint8_t *next_key = malloc(sizeof(uint8_t) * 16);
    uint8_t *prev_key_test = malloc(sizeof(uint8_t) * 16);

    // Test for all round 0->10
    for(int round = 0; round < 10; round++){
        printf("Round = %d\n", round);
        next_aes128_round_key(prev_key, next_key, 0);
        prev_aes128_round_key(next_key, prev_key_test, 0);

        for(int i = 0; i < 16; i++){
            if(prev_key[i] != prev_key_test[i]){
                printf("Our prev key calculation doesn't work\n");
                break;
            }
        }

        for(int i = 0; i < 16; i++){
            prev_key[i] = next_key[i];
        }

    }
    
    free(prev_key_test);
    free(prev_key);
    free(next_key);
    return 0;
}