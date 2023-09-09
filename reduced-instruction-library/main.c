#include <stdio.h>
#include <time.h>
#include <stdbool.h>


#include "tests.h"


//
typedef reg_t test_t;


// 
test_t multiplyTwoNumbers(test_t a, test_t b) {
   test_t result = 0;
   while (b > 0) {
      if (b & 1) {
         result += a;
         }
      a = a << 1;
      b = b >> 1;
   }
   return result;
}


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);

    // RUN TESTS
    run_all_tests();

    // CUSTOM TEST
    test_t ta = 0;
    test_t tb = 0;
    for (long long int a = 0; a < 65536; a++) {
        for (long long int b = 0; b < 65536; b++) {
            if (((test_t)(ta * tb) ^ (test_t)(multiplyTwoNumbers(ta, tb)))) {
                printf("%hd %hd\n", (test_t)(ta * tb), (test_t)(multiplyTwoNumbers(ta, tb)));
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
    reg_t b = 65923;
    reg_t c = multiplyTwoNumbers(a, b);
    printf("%hu %u\n", c, ((int)a * (int)b) % 65536);
    print_register(&c);

    // EXIT
    return 0;
}


