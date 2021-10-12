#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

/**
 * For a given block on a given key compute the following operation ARK + SR +SB + ARK
*/
void aesDemiTurnBackward(uint8_t block[AES_BLOCK_SIZE], uint8_t round_key[AES_BLOCK_SIZE], int lastround) {
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
    prev_aes128_round_key(round_key, prev_key, 3);

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
    //
}

int main(){
    printf("\nThis program implement the square attack on 3 rounds AES encryption\n");

    uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
    //Initialize allCipheredMessage to 0
    int i;
    for(i = 0; i < 256; i++){
        initBlockToZero(allCipheredMessage + 16 * i);
        allCipheredMessage[16*i] = i;
        oracleSquareAttack(allCipheredMessage + 16 * i);
    }
    
    //Init all possible byte for the key k 
    uint8_t *allPossibleKeyForEachByte = malloc(sizeof(uint8_t) * 16 * 256);
    for(i = 0; i < 256; i++){
        initBlockToValue(allPossibleKeyForEachByte + 16 * i, i);
    }

    distinguisherRightOrWrongKeyGuessed();

    free(allCipheredMessage);
    return 0;
}