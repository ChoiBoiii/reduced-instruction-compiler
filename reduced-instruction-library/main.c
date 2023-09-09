#include <stdio.h>
#include <time.h>
#include <stdbool.h>


// // 
// #define UINT_MULT(A, B) ({  \
//     reg_t a = A;            \
//     reg_t b = B;            \
//     reg_t result = 0;       \
//     while (b > 0) {         \
//         if (b & 1) {        \
//             result += a;    \
//         }                   \
//         a = BW_BSL(a, 1);   \
//         b = BW_BSR(b, 1);   \
//     }                       \
//     result;                 \
// })



// // 
// #define UINT_MULT(A, B) ({  \
//     reg_t a = A;            \
//     reg_t b = B;            \
//     reg_t result = 0;       \
//     while (b > 0) {         \
//         reg_t ifmask = BW_AND(b, 1);   \
//         ifmask |= (ifmask << 1); \
//         ifmask |= (ifmask << 2); \
//         ifmask |= (ifmask << 4); \
//         ifmask |= (ifmask << 8); \
//         result += BW_AND(a, ifmask);    \
//         a = BW_BSL(a, 1);   \
//         b = BW_BSR(b, 1);   \
//     }                       \
//     result;                 \
// })


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
    reg_t c = UINT_ADD(a, b);
    printf("%hu %u\n", c, ((int)a * (int)b) % 65536);
    print_register(&c);

    // EXIT
    return 0;
}


