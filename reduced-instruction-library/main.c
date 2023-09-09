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
    reg_t a = 19222;
    reg_t b = 65223;
    reg_t c = UINT_MULT(a, b);
    printf("%hu %u\n", c, ((int)a * (int)b) % 65536);
    print_register(&c);

    // EXIT
    return 0;
}


