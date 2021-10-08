#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

int main(){
    printf("\nThis program implement the square attack on 3 rounds AES encryption\n");

    uint8_t *allCipheredMessage = malloc(sizeof(uint8_t) * 16 * 256);
    
    distinguisherRightOrWrongKeyGuessed();

    free(allCipheredMessage);
    return 0;
}