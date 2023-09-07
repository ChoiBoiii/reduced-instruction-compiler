#include <stdio.h>
#include <time.h>
#include <stdbool.h>
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


    // RUN TESTS (TESTS OPERATE ON 16 BIT UINT EMULATED REGISTERS) ...

    // Config
    const long long int testIterCap = 66000;
    const reg_t tnum1 = 0;
    const reg_t tnum2 = 41746;
    const reg_t tnum3 = 7660;
    const reg_t tnumOnes = 0xFFFF;
    const reg_t tnumZeroes = 0x0000;
    reg_t testNum;


    // BITWISE OPERATORS ...

    // NAND
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((~(testNum & tnum1)) != (NAND(testNum, tnum1))) {
            printf("NAND Failed test\n");
            break;
        }
        if ((~(testNum & tnum2)) != (NAND(testNum, tnum2))) {
            printf("NAND Failed test\n");
            break;
        }
        if ((~(testNum & tnum3)) != (NAND(testNum, tnum3))) {
            printf("NAND Failed test\n");
            break;
        }
        if ((~(testNum & tnumOnes)) != (NAND(testNum, tnumOnes))) {
            printf("NAND Failed test\n");
            break;
        }
        if ((~(testNum & tnumZeroes)) != (NAND(testNum, tnumZeroes))) {
            printf("NAND Failed test\n");
            break;
        }
        testNum += 1;
    }

    // BSL
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        bool failed = false;
        for (int shiftAmmount = 0; shiftAmmount < sizeof(reg_t) * 8 + 1; shiftAmmount++) {
            if ((testNum << shiftAmmount) != (BSL(testNum, shiftAmmount))) {
                printf("BSL Failed test\n");
                failed = true;
                break;
            }
        }
        if (failed) {
            break;
        }
        testNum += 1;
    }

    // BSR
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        bool failed = false;
        for (int shiftAmmount = 0; shiftAmmount < sizeof(reg_t) * 8 + 1; shiftAmmount++) {
            if ((testNum >> shiftAmmount) != (BSR(testNum, shiftAmmount))) {
                printf("BSR Failed test\n");
                failed = true;
                break;
            }
        }
        if (failed) {
            break;
        }
        testNum += 1;
    }

    // AND
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum & tnum1) != (AND(testNum, tnum1))) {
            printf("AND Failed test\n");
            break;
        }
        if ((testNum & tnum2) != (AND(testNum, tnum2))) {
            printf("AND Failed test\n");
            break;
        }
        if ((testNum & tnum3) != (AND(testNum, tnum3))) {
            printf("AND Failed test\n");
            break;
        }
        if ((testNum & tnumOnes) != (AND(testNum, tnumOnes))) {
            printf("AND Failed test\n");
            break;
        }
        if ((testNum & tnumZeroes) != (AND(testNum, tnumZeroes))) {
            printf("AND Failed test\n");
            break;
        }
        testNum += 1;
    }

    // OR
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum | tnum1) != (OR(testNum, tnum1))) {
            printf("OR Failed test\n");
            break;
        }
        if ((testNum | tnum2) != (OR(testNum, tnum2))) {
            printf("OR Failed test\n");
            break;
        }
        if ((testNum | tnum3) != (OR(testNum, tnum3))) {
            printf("OR Failed test\n");
            break;
        }
        if ((testNum | tnumOnes) != (OR(testNum, tnumOnes))) {
            printf("OR Failed test\n");
            break;
        }
        if ((testNum | tnumZeroes) != (OR(testNum, tnumZeroes))) {
            printf("OR Failed test\n");
            break;
        }
        testNum += 1;
    }

    // NOT
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((~testNum) != (NOT(testNum))) {
            printf("NOT Failed test\n");
            break;
        }
        testNum += 1;
    }

    // XOR
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum ^ tnum1) != (XOR(testNum, tnum1))) {
            printf("XOR Failed test\n");
            break;
        }
        if ((testNum ^ tnum2) != (XOR(testNum, tnum2))) {
            printf("XOR Failed test\n");
            break;
        }
        if ((testNum ^ tnum3) != (XOR(testNum, tnum3))) {
            printf("XOR Failed test\n");
            break;
        }
        if ((testNum ^ tnumOnes) != (XOR(testNum, tnumOnes))) {
            printf("XOR Failed test\n");
            break;
        }
        if ((testNum ^ tnumZeroes) != (XOR(testNum, tnumZeroes))) {
            printf("XOR Failed test\n");
            break;
        }
        testNum += 1;
    }


    // EQUALITY OPERATORS ...

    // EQUAL0
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum == 0) != (EQUAL0(testNum))) {
            printf("EQUAL0 Failed test\n");
            break;
        }
        testNum += 1;
    }

    // NEQUAL0
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum != 0) != (NEQUAL0(testNum))) {
            printf("NEQUAL0 Failed test\n");
            break;
        }
        testNum += 1;
    }

    // NEQUAL
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum != tnum1) != (NEQUAL(testNum, tnum1))) {
            printf("NEQUAL Failed test\n");
            break;
        }
        if ((testNum != tnum2) != (NEQUAL(testNum, tnum2))) {
            printf("NEQUAL Failed test\n");
            break;
        }
        if ((testNum != tnum3) != (NEQUAL(testNum, tnum3))) {
            printf("NEQUAL Failed test\n");
            break;
        }
        if ((testNum != tnumOnes) != (NEQUAL(testNum, tnumOnes))) {
            printf("NEQUAL Failed test\n");
            break;
        }
        if ((testNum != tnumZeroes) != (NEQUAL(testNum, tnumZeroes))) {
            printf("NEQUAL Failed test\n");
            break;
        }
        testNum += 1;
    }

    // EQUAL
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum == tnum1) != (EQUAL(testNum, tnum1))) {
            printf("EQUAL Failed test\n");
            break;
        }
        if ((testNum == tnum2) != (EQUAL(testNum, tnum2))) {
            printf("EQUAL Failed test\n");
            break;
        }
        if ((testNum == tnum3) != (EQUAL(testNum, tnum3))) {
            printf("EQUAL Failed test\n");
            break;
        }
        if ((testNum == tnumOnes) != (EQUAL(testNum, tnumOnes))) {
            printf("EQUAL Failed test\n");
            break;
        }
        if ((testNum == tnumZeroes) != (EQUAL(testNum, tnumZeroes))) {
            printf("EQUAL Failed test\n");
            break;
        }
        testNum += 1;
    }

    // // GREATER_THAN
    // testNum = 0;
    // for (long long int i = 0; i < testIterCap; i++) {
    //     testNum += 1;
    //     if ((testNum > tnum1) != (GREATER_THAN(testNum, tnum1))) {
    //         printf("GREATER_THAN Failed test\n");
    //         break;
    //     }
    //     if ((testNum > tnum2) != (GREATER_THAN(testNum, tnum2))) {
    //         printf("GREATER_THAN Failed test\n");
    //         break;
    //     }
    //     if ((testNum > tnum3) != (GREATER_THAN(testNum, tnum3))) {
    //         printf("GREATER_THAN Failed test\n");
    //         break;
    //     }
    //     if ((testNum > tnumZeroes) != (GREATER_THAN(testNum, tnumZeroes))) {
    //         printf("GREATER_THAN Failed test\n");
    //         break;
    //     }
    //     if ((testNum > tnumOnes) != (GREATER_THAN(testNum, tnumOnes))) {
    //         printf("GREATER_THAN Failed test\n");
    //         break;
    //     }
    // }

    // UINT_ADD
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((reg_t)(testNum + tnum1) != (UINT_ADD(testNum, tnum1))) {
            printf("UINT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(testNum + tnum2) != (UINT_ADD(testNum, tnum2))) {
            printf("UINT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(testNum + tnum3) != (UINT_ADD(testNum, tnum3))) {
            printf("UINT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(testNum + tnumOnes) != (UINT_ADD(testNum, tnumOnes))) {
            printf("UINT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(testNum + tnumZeroes) != (UINT_ADD(testNum, tnumZeroes))) {
            printf("UINT_ADD Failed test\n");
            break;
        }
        testNum += 1;
    }

    // INT_ADD
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        testNum += 1;
        int16_t num = testNum;
        if ((num + (int16_t)tnum1) != ((int16_t)(INT_ADD(num, tnum1)))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((num + (int16_t)tnum2) != ((int16_t)(INT_ADD(num, tnum2)))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((num + (int16_t)tnum3) != ((int16_t)(INT_ADD(num, tnum3)))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((num + (int16_t)tnumZeroes) != ((int16_t)(INT_ADD(num, tnumZeroes)))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((num + (int16_t)tnumOnes) != ((int16_t)(INT_ADD(num, tnumOnes)))) {
            printf("INT_ADD Failed test\n");
            break;
        }
    }

    // EXIT
    return 0;
}


