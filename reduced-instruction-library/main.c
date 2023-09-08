#include <stdio.h>
#include <time.h>
#include <stdbool.h>


#include "tests.h"


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS
    run_all_tests();

    // EXIT
    return 0;
}


