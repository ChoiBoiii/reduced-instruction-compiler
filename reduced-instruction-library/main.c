#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// #define RIC_OVERRIDE_REGISTER_SIZE 16
// #define RIC_OVERRIDE_REGISTER_TYPE u_int16_t
// #include "nand.h"

#include "tests.h"


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // <- Enter code here
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    run_all_tests();

    // EXIT ...
    return 0;
}


