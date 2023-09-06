#include <stdio.h>
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

    reg_t x = 121;
    reg_t y = 121;
    reg_t result = ADD_UNSIGNED(x, y);
    printf("%d\n", result);
    // print_register(&result);

    // Return
    return 0;
}


