#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define RIC_OVERRIDE_REGISTER_SIZE 16
#define RIC_OVERRIDE_REGISTER_TYPE u_int16_t
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
    reg_t tnum1 = 0;
    reg_t tnum2 = 41746;
    reg_t tnum3 = 7660;
    reg_t tnumOnes = 0xFFFF;
    reg_t tnumZeroes = 0x0000;
    reg_t testNum;
    int16_t signedTestNum;


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
    signedTestNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((reg_t)(signedTestNum + tnum1) != (INT_ADD(signedTestNum, tnum1))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(signedTestNum + tnum2) != (INT_ADD(signedTestNum, tnum2))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(signedTestNum + tnum3) != (INT_ADD(signedTestNum, tnum3))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(signedTestNum + tnumOnes) != (INT_ADD(signedTestNum, tnumOnes))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        if ((reg_t)(signedTestNum + tnumZeroes) != (INT_ADD(signedTestNum, tnumZeroes))) {
            printf("INT_ADD Failed test\n");
            break;
        }
        signedTestNum += 1;
    }

    // INT_SIGN_INVERT
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if (((reg_t)(-testNum)) != (INT_SIGN_INVERT(testNum))) {
            reg_t a = -testNum;
            reg_t b = INT_SIGN_INVERT(testNum);
            printf("INT_SIGN_INVERT Failed test\n");
            printf("^ %hd %hd\n", a, b);
            print_register(&a);
            print_register(&b);
            break;
        }
        testNum += 1;
    }

    // UINT_SUB
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((reg_t)(testNum - tnum1) != (UINT_SUB(testNum, tnum1))) {
            printf("UINT_SUB Failed test\n");
            printf("^1 \n");
            break;
        }
        if ((reg_t)(testNum - tnum2) != (UINT_SUB(testNum, tnum2))) {
            printf("UINT_SUB Failed test\n");
            printf("^2 \n");
            break;
        }
        if ((reg_t)(testNum - tnum3) != (UINT_SUB(testNum, tnum3))) {
            printf("UINT_SUB Failed test\n");
            printf("^3 \n");
            break;
        }
        if ((reg_t)(testNum - tnumOnes) != (UINT_SUB(testNum, tnumOnes))) {
            printf("UINT_SUB Failed test\n");
            printf("^4 \n");
            break;
        }
        if ((reg_t)(testNum - tnumZeroes) != (UINT_SUB(testNum, tnumZeroes))) {
            printf("UINT_SUB Failed test\n");
            printf("^5 \n");
            break;
        }
        testNum += 1;
    }

    // INT_SUB
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((u_int16_t)(testNum - tnum1) ^ (INT_SUB(testNum, tnum1))) {
            printf("INT_SUB Failed test\n");
            printf("^1 \n");
            break;
        }
        if ((u_int16_t)(testNum - tnum2) ^ (INT_SUB(testNum, tnum2))) {
            printf("INT_SUB Failed test\n");
            printf("^2 \n");
            break;
        }
        if ((u_int16_t)(testNum - tnum3) ^ (INT_SUB(testNum, tnum3))) {
            int16_t a = (testNum - tnum3);
            int16_t b = INT_SUB(testNum, tnum3);
            int16_t c = (testNum - tnum3);
            int16_t d = INT_SUB(testNum, tnum3);
            printf("INT_SUB Failed test\n");
            printf("^3 \n");
            print_register(&a);
            print_register(&b);
            print_register(&c);
            print_register(&d);
            printf("%hd %hd\n", a, b);
            break;
        }
        if ((u_int16_t)(testNum - tnumOnes) ^ (INT_SUB(testNum, tnumOnes))) {
            printf("INT_SUB Failed test\n");
            printf("^4 \n");
            break;
        }
        if ((u_int16_t)(testNum - tnumZeroes) ^ (INT_SUB(testNum, tnumZeroes))) {
            printf("INT_SUB Failed test\n");
            printf("^5 \n");
            break;
        }
        testNum += 1;
    }

    // EXIT
    return 0;
}


