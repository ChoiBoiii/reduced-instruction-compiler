#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define RIC_OVERRIDE_REGISTER_SIZE 16
#define RIC_OVERRIDE_REGISTER_TYPE u_int16_t
#include "nand.h"


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


// Execute
int main() {

    // TIMER
    // clock_t t = clock();
    // <- Enter code here
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);


    //
    reg_t v = NOT(OR(NOT(0b1001100110011001), 0b1010101010101010));
    print_register(&v);

    // A = 1001
    // B = 1010
    // R = 0001
    // True if A & !B


    // EXIT ...
    return 0;
}


