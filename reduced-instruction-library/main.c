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

    // TIMER
    // clock_t t = clock();
    // <- Enter code here
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS ...

    // Config
    const long long int testIterCap = 66000;

    //
    long long int i = 0;
    reg_t testNum = 0;
    for (; i < testIterCap; i++) {
        testNum += 1;
        if ((testNum == 0) != (EQUAL0(testNum))) {
            printf("EQUAL0 Failed test\n");
        }
    }

    // EXIT
    return 0;
}


