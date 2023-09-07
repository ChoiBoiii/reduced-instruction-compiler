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
    reg_t a;
    for (int i = 0; i < 100000; i++) {
        a = INT_ADD(100, -(reg_t)i);
        printf("%lld %lld\n", (long long int)(100 - i), (long long int)(a));
        // print_register(&a);
    }
    INT_ADD(1 ,1 );

    // TIMER
    // clock_t t = clock();
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS

    // // Config
    // const long long int testIterCap = 66000;

    // //
    // long long int i = 0;
    // reg_t testNum = 0;
    // for (; i < testIterCap; i++) {
    //     testNum += 1;
    //     if ((testNum == 0) != (EQUAL0(testNum))) {
    //         printf("EQUAL0 Failed test\n");
    //     }
    // }

    // Exit
    return 0;
}


