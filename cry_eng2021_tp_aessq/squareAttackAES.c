#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

/**
 * For a given block on a given key compute the following operation ARK + SR + SB + ARK
*/
void aesDemiTurnBackward(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE], int8_t previous_turn) {
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
    prev_aes128_round_key(round_key, prev_key, previous_turn);
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
	/*for (i = 0; i < 16; i++)
	{
		block[i] ^= prev_key[i];
	}*/
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

		aesDemiTurnBackward(current_block, key, 3);
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
	int i, j;
    printf("\nThis program implement the square attack on 3 rounds and 1/2 AES encryption\n");
	uint8_t k[16] = {   
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
	uint8_t *save_k = malloc(sizeof(uint8_t) * 16 * 256);
	uint8_t *k_to_guess = malloc(sizeof(uint8_t) * 16 * 256);
	for(i = 0; i < 16; i++) {
		save_k[i] = k[i];
	}
    //Initialize allCipheredMessage to 0

	printf("\nK the key we'll use :\n");
	for(i = 0; i < 16; i++) {
		printf("%d ", k[i]);
	}
	printf("\n");
	for(i = 0; i < 256; i++){
        initBlockToZero(allCipheredMessage + 16 * i);
		allCipheredMessage[16*i] = i;
        //oracleSquareAttack(allCipheredMessage + 16 * i);
		aes128_enc(allCipheredMessage + 16 * i, k, 4, 0);
    }
	next_aes128_round_key(save_k, k, 0);
	next_aes128_round_key(k, k_to_guess, 1);
	next_aes128_round_key(k_to_guess, k, 2);
	next_aes128_round_key(k, k_to_guess, 3);
	printf("\nk_to_guess calculated : \n");
	for(i = 0; i < 16; i++) {
		printf("%d ", k_to_guess[i]);
	}
	printf("\n");

	uint8_t *xor_block = malloc(sizeof(uint8_t) * 16);
	for(i = 0; i < 256; i++){
        initBlockToZero(allCipheredMessage + 16 * i);
		allCipheredMessage[16*i] = i;
        //oracleSquareAttack(allCipheredMessage + 16 * i);
		aes128_enc(allCipheredMessage + 16 * i, k, 4, 0);
    }
	//aesDemiTurnBackward(allCipheredMessage, k_to_guess, 3);
	/*printf("\nallCipheredMessage :\n");
	for(i = 0; i < 256; i++) {
		for(j = 0; j < 16; j++) {
			printf("%d ", allCipheredMessage[i*16 + j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
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