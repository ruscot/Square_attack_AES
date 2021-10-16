#include <stdio.h>
#include <stdint.h>
#include "aes-128_enc.h"
#include "aes-128_enc.c"

int main(){
    printf("This test check if functions xtime and xtimeVariant works\n");
    uint8_t m = 1;
    printf("1 m = %d\n",m);
    m -= 1;
    printf("2 m = %d\n",m);
    m -= 1;
    printf("3 m = %d\n",m);
    
    uint8_t p = 0x1B;
    printf("p = %d\n", p);
    printf("p**2 = %d\n", xtime(p));
    p = 0xBB;
    printf("Now we take a multiple over F_2[X]/X^8 + X^4 + X^3 + X + 1\n");
    printf("p = %d\n", p);
    printf("p**2 = %d\n", xtime(p));

    p = 0x1B;
    printf("p = %d\n", p);
    printf("p**2 = %d\n", xtimeVariant(p));
    p = 0xBB;
    printf("Now we take a multiple over F_2[X]/X^8 + X^4 + X^3 + X + 1\n");
    printf("p = %d\n", p);
    printf("p**2 = %d\n", xtimeVariant(p));
    
    return 0;
}