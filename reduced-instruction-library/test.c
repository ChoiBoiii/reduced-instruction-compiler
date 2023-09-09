#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// Import tests
#include "tests.h"

// Execute
int main() {

    // RUN TESTS
    run_all_tests();

    // CUSTOM TEST
    signed_reg_type a = 3523;
    signed_reg_type b = -13;
    signed_reg_type c = INT_MULT(a, b);
    printf("%hd %hd\n", c, (reg_t)(((int)a * (int)b) % 65536));
    print_register(&c);

    reg_t _ric_out = 100;
    reg_t g = _ric_out;
    reg_t x = INT_GTHAN(g, 10);

    // EXIT
    return 0;
    
}


