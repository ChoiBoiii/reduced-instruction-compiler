#include <stdio.h>
#include <time.h>
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

    // reg_t x = 3456784275;
    // reg_t y = 65329991;
    // reg_t result = ADD_UNSIGNED(x, y);
    // printf("%u\n%u\n", result, x + y);
    // // print_register(&result);


    // Test
    for (reg_t i = 0; i < 10; i++) {
        printf("%llu\n", NEQUAL(i, i+1));
    }

    // TIMER
    // clock_t t = clock();
    // Insert here
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // Exit
    return 0;
}


