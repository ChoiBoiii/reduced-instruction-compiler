#include <stdio.h>


// The type to use as a 'register'
typedef unsigned char reg_t;


// Prints the given bits
void print_register(void* ptr) {
    printf("0b");
    for (int i = sizeof(reg_t) - 1; i >= 0; i--) {
        unsigned char byteToPrint = *(((char*)ptr) + i);
        unsigned char mask = 0b10000000;
        for (int j = 0; j < 8; j++) {
            printf((mask & byteToPrint) ? "1" : "0");
            mask = mask >> 1;
        }
    }
    printf("\n");
}


// Decomposed instructions
#define NAND(X, Y) ( ~(X & Y) )

#define AND(X, Y)  ( NAND(NAND(X, Y), NAND(X, Y)))
#define OR(X, Y)   ( NAND(NAND(X, X), NAND(Y, Y)))
#define NOT(X)     ( NAND(X, X))


// Execute
int main() {

    reg_t x = 0b1010;
    reg_t y = 0b1001;


    // Return
    return 0;
}


