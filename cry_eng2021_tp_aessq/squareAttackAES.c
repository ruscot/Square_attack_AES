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

void retrieveKey() {
    int i, j, n, nb_round;
    head_list *list_key_byte_head = malloc(sizeof(head_list));
	init_list_byte(list_key_byte_head);
    uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
    nb_round = 0;
    uint8_t *xor_block = malloc(sizeof(uint8_t) * 16);
	uint8_t *current_block = malloc(sizeof(uint8_t) * 16);
	uint8_t *guess_k = malloc(sizeof(uint8_t) * 16);
	uint8_t correspondingBytes[16] ={0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};
    while(check_all_byte_found(list_key_byte_head) == 0 && nb_round < 16){
        for(i = 0; i < 256; i++) {
            initBlockToZero(allCipheredMessage + 16 * i);
            allCipheredMessage[16*i + nb_round] = i ;
            oracleSquareAttack(allCipheredMessage + 16 * i);
        }
        //If we are in a new turn set in list of byte to 0
        init_new_turn_byte(list_key_byte_head);

        for(n = 0; n < 16; n++){
            if(check_one_byte_founc(list_key_byte_head, n) == 1){
                continue;
            }

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
                    if(nb_round == 0) {
                        add_byte_if_not_in(n, i, list_key_byte_head);
                    } else {
                        other_turn_set_in_list(n, i, list_key_byte_head);
                    }
                    
                }
            }
            remove_all_byte_not_in_turn(n, list_key_byte_head);
            check_byte_found_right_value(list_key_byte_head, n);
        }
        

        nb_round++;
    }
    print_head_list(list_key_byte_head);
    put_guess_byte_in_key(list_key_byte_head, guess_k);
    uint8_t *k = malloc(sizeof(uint8_t) * 16);

    prev_aes128_round_key(guess_k, k, 3);
    prev_aes128_round_key(k, guess_k, 2);
    prev_aes128_round_key(guess_k, k, 1);
    prev_aes128_round_key(k, guess_k, 0);
	
    printf("\nKey guessed : \n");
	for(i = 0; i < 16; i++){
        printf("%d ", guess_k[i]);
    }
    printf("\n");

    free(k);
	free(allCipheredMessage);
	free(xor_block);
	free(current_block);
	free(guess_k);
}

int main(){
    printf("\nThis program implement the square attack on 3 rounds and 1/2 AES encryption\n");
	retrieveKey();
    return 0;
}