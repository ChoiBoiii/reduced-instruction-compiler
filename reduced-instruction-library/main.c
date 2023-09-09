#include <stdio.h>
#include <time.h>
#include <stdbool.h>


//
#include "tests.h"


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS
    run_all_tests();

    //
    // reg_t a = EXTRACT_MSB_EQ_HELPER(0b0000000000000000);
    // reg_t b = EXTRACT_MSB_EQ_HELPER(0b0000000000100000);
    // reg_t c = EXTRACT_MSB_EQ_HELPER(0b0000010010011101);
    // reg_t d = EXTRACT_MSB_EQ_HELPER(0b1000000000000000);
    // reg_t e = EXTRACT_MSB_EQ_HELPER(0b1010000011101110);
    // reg_t f = EXTRACT_MSB_EQ_HELPER(0b0000000100001000);
    // print_register(&a);
    // print_register(&b);
    // print_register(&c);
    // print_register(&d);
    // print_register(&e);
    // print_register(&f);

    // //
    // reg_t a = 19222;
    // reg_t b = 65223;
    // reg_t c = UINT_ADD(a, b);
    // printf("%hu %u\n", c, ((int)a * (int)b) % 65536);
    // print_register(&c);

    // EXIT
    return 0;
}


