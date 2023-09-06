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

#define AND(X, Y)  ( NAND(NAND(X, Y), NAND(X, Y)) )
#define OR(X, Y)   ( NAND(NAND(X, X), NAND(Y, Y)) )
#define NOT(X)     ( NAND(X, X)                   )
#define XOR(X, Y)  ( AND(OR(X, Y), NAND(X, Y))    )

reg_t ADD(X, Y) {
    return ((AND(X, Y) << 1) != 0) ? ADD((AND(X, Y) << 1), XOR(X, Y)) : XOR(X, Y);
}

// reg_t ADD(X, Y) {
//     reg_t keep = (X & Y) << 1;
//     reg_t res = X ^ Y;
//     return (keep) ? ADD(keep, res) : res;
// }


// Execute
int main() {

    reg_t x = 121;
    reg_t y = 121;
    reg_t result = ADD(x, y);
    printf("%d\n", result);
    // print_register(&result);

    // Return
    return 0;
}


