#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

void aes_round2(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE], int lastround, uint8_t block_without_key[AES_BLOCK_SIZE])
{
	int i;
	uint8_t tmp;

	/*
	 * SubBytes + ShiftRow
	 */
	/* Row 0 */
	block[ 0] = S[block[ 0]];
	block[ 4] = S[block[ 4]];
	block[ 8] = S[block[ 8]];
	block[12] = S[block[12]];
	/* Row 1 */
	tmp = block[1];
	block[ 1] = S[block[ 5]];
	block[ 5] = S[block[ 9]];
	block[ 9] = S[block[13]];
	block[13] = S[tmp];
	/* Row 2 */
	tmp = block[2];
	block[ 2] = S[block[10]];
	block[10] = S[tmp];
	tmp = block[6];
	block[ 6] = S[block[14]];
	block[14] = S[tmp];
	/* Row 3 */
	tmp = block[15];
	block[15] = S[block[11]];
	block[11] = S[block[ 7]];
	block[ 7] = S[block[ 3]];
	block[ 3] = S[tmp];

	/*
	 * MixColumns
	 */
	for (i = lastround; i < 16; i += 4) /* lastround = 16 if it is the last round, 0 otherwise */
	{
		uint8_t *column = block + i;
		uint8_t tmp2 = column[0];
		tmp = column[0] ^ column[1] ^ column[2] ^ column[3];

		column[0] ^= tmp ^ xtime(column[0] ^ column[1]);
		column[1] ^= tmp ^ xtime(column[1] ^ column[2]);
		column[2] ^= tmp ^ xtime(column[2] ^ column[3]);
		column[3] ^= tmp ^ xtime(column[3] ^ tmp2);
	}
	
	for (i = 0; i < 16; i++)
	{
		block_without_key[i] = block[i];
	}

	/*
	 * AddRoundKey
	 */
	for (i = 0; i < 16; i++)
	{
		block[i] ^= round_key[i];
	}
}

void applyKeyInverse(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE], uint8_t prev_key[AES_BLOCK_SIZE]) {
    int i;
	//uint8_t tmp;
    prev_aes128_round_key(round_key, prev_key, 0);
    /*
	 * AddRoundKey
	 */
	for (i = 0; i < 16; i++)
	{
		block[i] ^= prev_key[i];
	}

    
}

/**
 * For a given block on a given key compute the following operation ARK + SR +SB + ARK
*/
void aesDemiTurnBackward(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE]) {
    //int i;
	uint8_t tmp;

	/*
	 * SubBytes + ShiftRow
	 */

    block[ 0] = Sinv[block[ 0]];
	block[ 4] = Sinv[block[ 4]];
	block[ 8] = Sinv[block[ 8]];
	block[12] = Sinv[block[12]];

	tmp = block[9];
    block[ 9] = Sinv[block[ 5]];
    block[ 5] = Sinv[block[ 1]];
    block[ 1] = Sinv[block[13]];
    block[13] = Sinv[tmp];

	tmp = block[2];
	block[ 2] = Sinv[block[10]];
	block[10] = Sinv[tmp];
	tmp = block[6];
	block[ 6] = Sinv[block[14]];
	block[14] = Sinv[tmp];

	tmp = block[15];
    
    block[15] = Sinv[block[ 3]];
    block[ 3] = Sinv[block[ 7]];
    block[ 7] = Sinv[block[11]];
    block[11] = Sinv[tmp];
	
}

int main(){
    printf("\nThis program test the reverse of a demi turn\n\n");
    uint8_t *prev_key = malloc(sizeof(uint8_t) * 16);
    int i;
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
    uint8_t *previous_block = malloc(sizeof(uint8_t) * 16);
    previous_block[ 0] = 0x2b;
    previous_block[ 1] = 0x8e;
    previous_block[ 2] = 0x15;
    previous_block[ 3] = 0x26;
    previous_block[ 4] = 0x88;
    previous_block[ 5] = 0x0e;
    previous_block[ 6] = 0xd2;
    previous_block[ 7] = 0xa6;
    previous_block[ 8] = 0xbb;
    previous_block[ 9] = 0x57;
    previous_block[10] = 0x15;
    previous_block[11] = 0x58;
    previous_block[12] = 0x09;
    previous_block[13] = 0x8f;
    previous_block[14] = 0x4f;
    previous_block[15] = 0x9c;
    uint8_t *block_without_key = malloc(sizeof(uint8_t) * 16);
    aes_round2(block, prev_key, 16, block_without_key);
    next_aes128_round_key(prev_key, next_key, 0);
    uint8_t *prev_key_test = malloc(sizeof(uint8_t) * 16);
    applyKeyInverse(block, next_key, prev_key_test);
    //Check if the retrieve key is the good one
    for(i = 0; i < 16; i++){
        if(prev_key_test[i] != prev_key[i]){
            printf("Problem in previous key !!\n");
            break;
        }
    }
    //Check if the block decrypt by the key is the good one
    for(i = 0; i < 16; i++){
        if(block_without_key[i] != block[i]){
            printf("Problem in block without key !!\n");
            break;
        }
    }
    aesDemiTurnBackward(block, next_key);
    //Check if the block decrypt by the subbyte + shift row is the same
    for(i = 0; i < 16; i++){
        if(previous_block[i] != block[i]){
            printf("Problem in code decryption !!\n");
            break;
        }
    }

    free(previous_block);
    free(block);
    free(next_key);
    free(prev_key_test);
    free(prev_key);
    return 0;
}