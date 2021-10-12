#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

/**
 * For a given block on a given key compute the following operation ARK + SR + SB + ARK
*/
void aesDemiTurnBackward(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE]) {
    int i;
	uint8_t tmp;

    /*
	 * AddRoundKey
	 */
	for (i = 0; i < 16; i++)
	{
		block[i] ^= round_key[i];
	}

    uint8_t* prev_key = malloc(sizeof(uint8_t) * 16);
    prev_aes128_round_key(round_key, prev_key, 0);
	printf("\n");
	for(i = 0; i < 16; i++) {
		printf("%d ", prev_key[i]);
	}
	/*
	 * SubBytes + ShiftRow
	 */
	/* Row 0 */
	block[ 0] = Sinv[block[ 0]];
	block[ 4] = Sinv[block[ 4]];
	block[ 8] = Sinv[block[ 8]];
	block[12] = Sinv[block[12]];
	/* Row 1 */
	tmp = block[9];
    block[ 9] = Sinv[block[ 5]];
    block[ 5] = Sinv[block[ 1]];
    block[ 1] = Sinv[block[13]];
    block[13] = Sinv[tmp];
	/* Row 2 */
	tmp = block[2];
	block[ 2] = Sinv[block[10]];
	block[10] = Sinv[tmp];
	tmp = block[6];
	block[ 6] = Sinv[block[14]];
	block[14] = Sinv[tmp];
	/* Row 3 */
	tmp = block[15];
    block[15] = Sinv[block[ 3]];
    block[ 3] = Sinv[block[ 7]];
    block[ 7] = Sinv[block[11]];
    block[11] = Sinv[tmp];
    
	/*
	 * AddRoundKey
	 */
	for (i = 0; i < 16; i++)
	{
		block[i] ^= prev_key[i];
	}
}

/**
 * For a given value of the key byte test on all key bytes if it's value correspond to a possible 
 * byte of the correct key 
*/
void guessCorrectKeyByte(uint8_t key[AES_BLOCK_SIZE], uint8_t block[AES_BLOCK_SIZE*256], uint8_t keyByte) {
	printf("Start of guess correct bytes\n");
    //Init the key to the S
	int i, j;
	for(i = 0; i < 16; i++){
		key[i] = 0;
	}
	key[0] = keyByte;
	//For all block get the last value and compute the sum to test if it's equal to 0
	uint8_t* sum_block = malloc(sizeof(uint8_t) * 16);
	uint8_t* current_block = malloc(sizeof(uint8_t) * 16);
	initBlockToZero(sum_block);
	for(i = 0; i < 256; i++){
		for(j = 0; j < 16; j++) {
			current_block[j] = block[i*16 + j];
		}

		aesDemiTurnBackward(current_block, key);
		(*sum_block) ^= (*current_block);
	}/*
	for(j = 0; j < 16; j++) {
		printf("%d ", sum_block[j]);
	}
	printf("\n");*/
	for(j = 0; j < 16; j++) {
		if(sum_block[j] == 0){
			printf("\nKeyBytes %d for the bytes %d", keyByte, j);
		}
	}
	free(sum_block);
	free(current_block);
}

int main(){
    printf("\nThis program implement the square attack on 3 rounds and 1/2 AES encryption\n");

    uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
	uint8_t *prev_key = malloc(sizeof(uint8_t) * 16);
	//uint8_t *other_key = malloc(sizeof(uint8_t) * 16);
	//uint8_t *third_key = malloc(sizeof(uint8_t) * 16);
    prev_key[ 0] = 0x85;
    prev_key[ 1] = 0x7e;
    prev_key[ 2] = 0x15;
    prev_key[ 3] = 0x16;
    prev_key[ 4] = 0x28;
    prev_key[ 5] = 0xae;
    prev_key[ 6] = 0xd2;
    prev_key[ 7] = 0xa6;
    prev_key[ 8] = 0x01;
    prev_key[ 9] = 0x85;
    prev_key[10] = 0x15;
    prev_key[11] = 0x88;
    prev_key[12] = 0x09;
    prev_key[13] = 0xcf;
    prev_key[14] = 0x4f;
    prev_key[15] = 0x3c;
	uint8_t *block = malloc(sizeof(uint8_t) * 16);
	block[ 0] = 0x2b;
    block[ 1] = 0x8e;
    block[ 2] = 0x15;
    block[ 3] = 0x26;
    block[ 4] = 0x88;
    block[ 5] = 0x0e;
    block[ 6] = 0xd2;
    block[ 7] = 0xa6;
    block[ 8] = 0xbb;
    block[ 9] = 0x57;
    block[10] = 0x15;
    block[11] = 0x58;
    block[12] = 0x09;
    block[13] = 0x8f;
    block[14] = 0x4f;
    block[15] = 0x9c;
	uint8_t *next_key = malloc(sizeof(uint8_t) * 16);
	uint8_t *save_block = malloc(sizeof(uint8_t) * 16);
	save_block[ 0] = 0x2b;
    save_block[ 1] = 0x8e;
    save_block[ 2] = 0x15;
    save_block[ 3] = 0x26;
    save_block[ 4] = 0x88;
    save_block[ 5] = 0x0e;
    save_block[ 6] = 0xd2;
    save_block[ 7] = 0xa6;
    save_block[ 8] = 0xbb;
    save_block[ 9] = 0x57;
    save_block[10] = 0x15;
    save_block[11] = 0x58;
    save_block[12] = 0x09;
    save_block[13] = 0x8f;
    save_block[14] = 0x4f;
    save_block[15] = 0x9c;
    //Initialize allCipheredMessage to 0
    int i, j;
	printf("\n");
	for(i = 0; i < 16; i++) {
		printf("%d ", prev_key[i]);
	}
	aes128_enc(block, prev_key, 1, 0);
	next_aes128_round_key(prev_key, next_key, 0);	
	aesDemiTurnBackward(block, next_key);
	printf("\n");
	for(i = 0; i < 16; i++) {
		printf("%d ", block[i]);
	}
	printf("\n");
	for(i = 0; i < 16; i++) {
		printf("%d ", save_block[i]);
	}


    /*for(i = 0; i < 256; i++){
        initBlockToZero(allCipheredMessage + 16 * i);
		allCipheredMessage[16*i+5] = i;
        //oracleSquareAttack(allCipheredMessage + 16 * i);
		aes128_enc(allCipheredMessage + 16 * i, prev_key, 4, 0);
    }
    next_aes128_round_key(prev_key, third_key, 0);
	next_aes128_round_key(third_key, other_key, 1);
	next_aes128_round_key(other_key, third_key, 2);
	printf("\nThird key :\n");
	for(i = 0; i < 16; i++){
        printf("%d ", third_key[i]);
    }
	printf("\n");
    //Init all possible byte for the key k 
    uint8_t *allPossibleKeyForEachByte = malloc(sizeof(uint8_t) * 16 * 256);
    for(i = 0; i < 256; i++){
        initBlockToValue(allPossibleKeyForEachByte + 16 * i, i);
    }
	guessCorrectKeyByte(prev_key, allCipheredMessage, 5);
	*/
    //distinguisherRightOrWrongKeyGuessed();

    free(allCipheredMessage);
	//free(third_key);
    return 0;
}