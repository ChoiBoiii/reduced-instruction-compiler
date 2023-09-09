#include <stdio.h>
#include <time.h>
#include <stdbool.h>


#include "tests.h"


// 
#define UINT_MULT(A, B) ({  \
    reg_t a = A;            \
    reg_t b = B;            \
    reg_t result = 0;       \
    while (b > 0) {         \
        if (b & 1) {        \
            result += a;    \
        }                   \
        a = BW_BSL(a, 1);   \
        b = BW_BSR(b, 1);   \
    }                       \
    result;                 \
})


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS
    run_all_tests();

    // CUSTOM TEST
    reg_t ta = 0;
    reg_t tb = 0;
    for (long long int a = 0; a < 65540; a++) {
        for (long long int b = 0; b < 65540; b++) {
            if (((reg_t)(ta * tb) ^ (UINT_MULT(ta, tb)))) {
                printf("%hd %hd\n", (reg_t)(ta * tb), (reg_t)(UINT_MULT(ta, tb)));
                break;
            }
            tb += 1;
        }
        ta += 1;
        if (ta % 1000 == 0) {
            printf("ping\n");
        }
    }

    //
    reg_t a = 19222;
    reg_t b = 65223;
    reg_t c = UINT_ADD(a, b);
    printf("%hu %u\n", c, ((int)a * (int)b) % 65536);
    print_register(&c);

    // EXIT
    return 0;
}


