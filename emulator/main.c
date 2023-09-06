#include <stdio.h>


// The type to use as a 'register'
#define reg_t unsigned int64_t;


// 
void print_bits(void* ptr, int numBytes) {
    printf("0b");
    for (int i = numBytes - 1; i >= 0; i--) {
        unsigned char byteToPrint = *(((char*)ptr) + i);
        unsigned char mask = 0b10000000;
        for (int j = 0; j < 8; j++) {
            printf((mask & byteToPrint) ? "1" : "0");
            mask = mask >> 1;
        }
    }
    printf("\n");
}


//
#define NAND(x, y) ( ~(x &  y) )
#define AND(x, y)  ( NAND(NAND(x, y), NAND(x, y)) )
#define NOT(x)     ( NAND(x, x) )


// Execute
int main() {

    // 
    int32_t t = 0b11110000111111110000000000001111;
    print_bits(&t, 4);

    // Return
    return 0;
}


