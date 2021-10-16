#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"
#include "struct_function.c"

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
}

int main(){
	int i, j, n;
    printf("\nThis program implement the square attack on 3 rounds and 1/2 AES encryption\n");
	head_list *list_key_byte_head = malloc(sizeof(head_list));
	init_list_byte(list_key_byte_head);
	uint8_t k[16] = {   
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
	uint8_t *save_k = malloc(sizeof(uint8_t) * 16);
	uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
	uint8_t *k_to_guess = malloc(sizeof(uint8_t) * 16);
	for(i = 0; i < 16; i++) {
		save_k[i] = k[i];
	}
    //Initialize allCipheredMessage to 0

	printf("\nK the key we'll use :\n");
	for(i = 0; i < 256; i++) {
        initBlockToZero(allCipheredMessage + 16 * i);
		allCipheredMessage[16*i + 2] = i ;
		aes128_enc(allCipheredMessage + 16 * i, k, 4, 0);
    }
	next_aes128_round_key(save_k, k, 0);
	next_aes128_round_key(k, k_to_guess, 1);
	next_aes128_round_key(k_to_guess, k, 2);
	next_aes128_round_key(k, k_to_guess, 3);
	printf("\nk calculated : \n");
	for(i = 0; i < 16; i++) {
		printf("%d ", k[i]);
	}
	printf("\n");
	printf("\nk_to_guess calculated : \n");
	for(i = 0; i < 16; i++) {
		printf("%d ", k_to_guess[i]);
	}
	printf("\n");

	//If the xor is equal to 0 the byte of the key is correct 
	uint8_t *xor_block = malloc(sizeof(uint8_t) * 16);
	uint8_t *current_block = malloc(sizeof(uint8_t) * 16);
	uint8_t *guess_k = malloc(sizeof(uint8_t) * 16);

	uint8_t correspondingBytes[16] ={0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};
	for(n = 0; n < 16; n++){
		for(i = 0; i < 256; i++){
			initBlockToZero(guess_k);
			guess_k[n] = i;
			initBlockToZero(xor_block);
			for(j = 0; j < 256; j++){
				memcpy(current_block, allCipheredMessage + j * 16, sizeof(uint8_t) * 16);
				aesDemiTurnBackward(current_block, guess_k);
				xor_block[correspondingBytes[n]] ^= current_block[correspondingBytes[n]];
			}
			if(xor_block[correspondingBytes[n]] == 0){
				add_byte_if_not_in(n, i, list_key_byte_head);
			}
		}
		check_byte_found_right_value(list_key_byte_head, n);
    }
	print_head_list(list_key_byte_head);
    
	
	free(allCipheredMessage);
	free(xor_block);
	free(current_block);
	free(guess_k);
	free(save_k);
	free(k_to_guess);
    return 0;
}