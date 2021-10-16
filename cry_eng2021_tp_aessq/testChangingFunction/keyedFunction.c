#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "distinguisher.c"

int main(){
    printf("\nThis program implement the keyed function F defined as below :\n"
        "Given k1, k2 and x, F(k1||k2, x) = E(k1, x) xor E(k2, x)\n"
        "For our case E is equal to round of AES with ki the key and x the message to cipher\n"
        "To be non trivial we need k1 != k2 because if k1 = k2 => E(k1, x) = E(k2, x) so F(k1||k2, x) = 0\n");
    
    uint8_t *k = malloc(sizeof(uint8_t) * 32);
    
    k[ 0] = 0x2b;
    k[ 1] = 0x7e;
    k[ 2] = 0x15;
    k[ 3] = 0x16;
    k[ 4] = 0x28;
    k[ 5] = 0xae;
    k[ 6] = 0xd2;
    k[ 7] = 0xa6;
    k[ 8] = 0xab;
    k[ 9] = 0xf7;
    k[10] = 0x15;
    k[11] = 0x88;
    k[12] = 0x09;
    k[13] = 0xcf;
    k[14] = 0x4f;
    k[15] = 0x3c;
    k[16] = 0x45;
    k[17] = 0x58;
    k[18] = 0x0a;
    k[19] = 0x16;
    k[20] = 0x08;
    k[21] = 0xa1;
    k[22] = 0x82;
    k[23] = 0xa6;
    k[24] = 0x9b;
    k[25] = 0xd7;
    k[26] = 0x15;
    k[27] = 0x08;
    k[28] = 0x59;
    k[29] = 0xbf;
    k[30] = 0xaf;
    k[31] = 0xbc;
    
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
    distinguisherRandomPermutationOrAESEncryption();
    free(k);
    free(x);
    return 0;
}