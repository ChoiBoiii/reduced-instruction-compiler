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
    clock_t t = clock();
    reg_t i = 0;
    for (; i < 100000000; i++) {
    }
    printf("%d\n", i);
    t = clock() - t;
    printf("%f seconds\n", ((float)t) / CLOCKS_PER_SEC);


    // Return
    return 0;
}


