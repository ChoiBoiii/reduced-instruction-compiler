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


// DEFINE TESTS ...

// CONFIG
const long long int testIterCap = 70000;
reg_t tnums[] = {1, 41746, 7660, 7660, 0xFFFF, 0x0000};
const int numTestNums = sizeof(tnums) / sizeof(reg_t);

// Tests
bool unit_test_BW_NAND(reg_t n1, reg_t n2)    {return (~(n1 & n2)) ^ (BW_NAND(n1, n2));}
bool unit_test_BW_BSL(reg_t n1, reg_t n2)     {return ((n1 << n2) ^ (BW_BSL(n1, n2)));}
bool unit_test_BW_BSR(reg_t n1, reg_t n2)     {return ((n1 >> n2) ^ (BW_BSR(n1, n2)));}
bool unit_test_BW_AND(reg_t n1, reg_t n2)     {return ((n1 & n2) ^ (BW_AND(n1, n2)));}
bool unit_test_BW_OR(reg_t n1, reg_t n2)      {return ((n1 | n2) ^ (BW_OR(n1, n2)));}
bool unit_test_BW_NOT(reg_t n1, reg_t n2)     {return ((~n1) ^ (BW_NOT(n1)));}
bool unit_test_BW_XOR(reg_t n1, reg_t n2)     {return ((n1 ^ n2) ^ (BW_XOR(n1, n2)));}
bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)   {return ((n1 == n2) ^ (BW_EQUAL(n1, n2)));}

// RUNS GIVEN TEST
bool run_test(char* testName, bool (*func)(reg_t, reg_t)) {
    reg_t tnum1 = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        for (int j = 0; j < numTestNums; j++) {
            reg_t tnum2 = tnums[j];
            if (func(tnum1, tnum2)) {
                printf("Test [%s] failed\n", testName);
                return true;
            }
        }
        tnum1++;
    }
    return false;
}
bool run_bitshift_test(char* testName, bool (*func)(reg_t, reg_t)) {
    reg_t tnum1 = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        for (long long int j = 0; j < REGISTER_SIZE_BITS + 8; j++) {
            reg_t tnum2 = j;
            if (func(tnum1, tnum2)) {
                printf("Test [%s] failed\n", testName);
                return true;
            }
        }
        tnum1++;
    }
    return false;
}


// Execute`
int main() {

    // TIMER
    // clock_t t = clock();
    // <- Enter code here
    // printf("%f seconds\n", ((float)(clock() - t)) / CLOCKS_PER_SEC);


    // RUN TESTS (TESTS OPERATE ON 16 BIT UINT EMULATED REGISTERS) ...

    reg_t tnum1 = 1;
    reg_t tnum2 = 41746;
    reg_t tnum3 = 7660;
    reg_t tnumOnes = 0xFFFF;
    reg_t tnumZeroes = 0x0000;
    reg_t testNum;
    int16_t signedTestNum;
    char* testName;

 
    // BITWISE OPERATORS ...

    // BW_NAND
    run_test("BW_NAND", unit_test_BW_NAND);

    // BW_BSL
    run_bitshift_test("BW_BSL", unit_test_BW_BSL);

    // BW_BSR
    run_bitshift_test("BW_BSR", unit_test_BW_BSR);

    // BW_AND
    run_test("BW_AND", unit_test_BW_AND);

    // BW_OR
    run_test("BW_OR", unit_test_BW_OR);

    // BW_NOT
    run_test("BW_NOT", unit_test_BW_NOT);

    // BW_XOR
    run_test("BW_XOR", unit_test_BW_XOR);


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

    // BW_EQUAL
    run_test("BW_EQUAL", unit_test_BW_EQUAL);

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

    // UINT_GTHAN
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum > tnum1) ^ (UINT_GTHAN(testNum, tnum1))) {
            printf("UINT_GTHAN Failed test\n");
            printf("^1\n");
            printf("%hu %hu\n", testNum, tnum1);
            reg_t a = (testNum > tnum1);
            reg_t b = (UINT_GTHAN(testNum, tnum1));
            print_register(&a);
            print_register(&b);
            break;
        }
        if ((testNum > tnum2) ^ (UINT_GTHAN(testNum, tnum2))) {
            printf("UINT_GTHAN Failed test\n");
            printf("^2\n");
            printf("%hu %hu\n", testNum, tnum2);
            reg_t a = (testNum > tnum1);
            reg_t b = (UINT_GTHAN(testNum, tnum2));
            print_register(&a);
            print_register(&b);
            break;
        }
        if ((testNum > tnum3) ^ (UINT_GTHAN(testNum, tnum3))) {
            printf("UINT_GTHAN Failed test\n");
            printf("^3\n");
            break;
        }
        if ((testNum > tnumOnes) ^ (UINT_GTHAN(testNum, tnumOnes))) {
            printf("UINT_GTHAN Failed test\n");
            printf("^4\n");
            break;
        }
        if ((testNum > tnumZeroes) ^ (UINT_GTHAN(testNum, tnumZeroes))) {
            printf("UINT_GTHAN Failed test\n");
            printf("^5\n");
            break;
        }
        testNum += 1;
    }

    // UINT_GEQUAL
    testNum = 0;
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum >= tnum1) ^ (UINT_GEQUAL(testNum, tnum1))) {
            printf("UINT_GEQUAL Failed test\n");
            printf("^1\n");
            break;
        }
        if ((testNum >= tnum2) ^ (UINT_GEQUAL(testNum, tnum2))) {
            printf("UINT_GEQUAL Failed test\n");
            printf("^2\n");
            break;
        }
        if ((testNum >= tnum3) ^ (UINT_GEQUAL(testNum, tnum3))) {
            printf("UINT_GEQUAL Failed test\n");
            printf("^3\n");
            break;
        }
        if ((testNum >= tnumOnes) ^ (UINT_GEQUAL(testNum, tnumOnes))) {
            printf("UINT_GEQUAL Failed test\n");
            printf("^4\n");
            break;
        }
        if ((testNum >= tnumZeroes) ^ (UINT_GEQUAL(testNum, tnumZeroes))) {
            printf("UINT_GEQUAL Failed test\n");
            printf("^5\n");
            break;
        }
        testNum += 1;
    }

    // UINT_LTHAN
    testNum = 0;
    testName = "UINT_LTHAN";
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum < tnum1) ^ (UINT_LTHAN(testNum, tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((testNum < tnum2) ^ (UINT_LTHAN(testNum, tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((testNum < tnum3) ^ (UINT_LTHAN(testNum, tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((testNum < tnumOnes) ^ (UINT_LTHAN(testNum, tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((testNum < tnumZeroes) ^ (UINT_LTHAN(testNum, tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        testNum += 1;
    }

    // UINT_LEQUAL
    testNum = 0;
    testName = "UINT_LEQUAL";
    for (long long int i = 0; i < testIterCap; i++) {
        if ((testNum <= tnum1) ^ (UINT_LEQUAL(testNum, tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((testNum <= tnum2) ^ (UINT_LEQUAL(testNum, tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((testNum <= tnum3) ^ (UINT_LEQUAL(testNum, tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((testNum <= tnumOnes) ^ (UINT_LEQUAL(testNum, tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((testNum <= tnumZeroes) ^ (UINT_LEQUAL(testNum, tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        testNum += 1;
    }

    // INT_GTHAN
    testNum = 0;
    testName = "INT_GTHAN";
    for (long long int i = 0; i < testIterCap; i++) {
        int16_t tnum = (int16_t)testNum;
        if ((tnum > (int16_t)tnum1) ^ (INT_GTHAN(tnum, (int16_t)tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((tnum > (int16_t)tnum2) ^ (INT_GTHAN(tnum, (int16_t)tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((tnum > (int16_t)tnum3) ^ (INT_GTHAN(tnum, (int16_t)tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((tnum > (int16_t)tnumOnes) ^ (INT_GTHAN(tnum, (int16_t)tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((tnum > (int16_t)tnumZeroes) ^ (INT_GTHAN(tnum, (int16_t)tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        if ((tnum > (int16_t)(-12453)) ^ (INT_GTHAN(tnum, (int16_t)(-12453)))) {
            printf("%s Failed test\n^6\n", testName);
            break;   
        }
        testNum += 1;
    }

    // INT_GEQUAL
    testNum = 0;
    testName = "INT_GEQUAL";
    for (long long int i = 0; i < testIterCap; i++) {
        int16_t tnum = (int16_t)testNum;
        if ((tnum >= (int16_t)tnum1) ^ (INT_GEQUAL(tnum, (int16_t)tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((tnum >= (int16_t)tnum2) ^ (INT_GEQUAL(tnum, (int16_t)tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((tnum >= (int16_t)tnum3) ^ (INT_GEQUAL(tnum, (int16_t)tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((tnum >= (int16_t)tnumOnes) ^ (INT_GEQUAL(tnum, (int16_t)tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((tnum >= (int16_t)tnumZeroes) ^ (INT_GEQUAL(tnum, (int16_t)tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        if ((tnum >= (int16_t)(-12453)) ^ (INT_GEQUAL(tnum, (int16_t)(-12453)))) {
            printf("%s Failed test\n^6\n", testName);
            break;   
        }
        testNum += 1;
    }

    // INT_LTHAN
    testNum = 0;
    testName = "INT_LTHAN";
    for (long long int i = 0; i < testIterCap; i++) {
        int16_t tnum = (int16_t)testNum;
        if ((tnum < (int16_t)tnum1) ^ (INT_LTHAN(tnum, (int16_t)tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((tnum < (int16_t)tnum2) ^ (INT_LTHAN(tnum, (int16_t)tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((tnum < (int16_t)tnum3) ^ (INT_LTHAN(tnum, (int16_t)tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((tnum < (int16_t)tnumOnes) ^ (INT_LTHAN(tnum, (int16_t)tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((tnum < (int16_t)tnumZeroes) ^ (INT_LTHAN(tnum, (int16_t)tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        if ((tnum < (int16_t)(-12453)) ^ (INT_LTHAN(tnum, (int16_t)(-12453)))) {
            printf("%s Failed test\n^6\n", testName);
            break;   
        }
        testNum += 1;
    }

    // INT_LEQUAL
    testNum = 0;
    testName = "INT_LEQUAL";
    for (long long int i = 0; i < testIterCap; i++) {
        int16_t tnum = (int16_t)testNum;
        if ((tnum <= (int16_t)tnum1) ^ (INT_LEQUAL(tnum, (int16_t)tnum1))) {
            printf("%s Failed test\n^1\n", testName);
            break;
        }
        if ((tnum <= (int16_t)tnum2) ^ (INT_LEQUAL(tnum, (int16_t)tnum2))) {
            printf("%s Failed test\n^2\n", testName);
            break;
        }
        if ((tnum <= (int16_t)tnum3) ^ (INT_LEQUAL(tnum, (int16_t)tnum3))) {
            printf("%s Failed test\n^3\n", testName);
            break;
        }
        if ((tnum <= (int16_t)tnumOnes) ^ (INT_LEQUAL(tnum, (int16_t)tnumOnes))) {
            printf("%s Failed test\n^4\n", testName);
            break;
        }
        if ((tnum <= (int16_t)tnumZeroes) ^ (INT_LEQUAL(tnum, (int16_t)tnumZeroes))) {
            printf("%s Failed test\n^5\n", testName);
            break;
        }
        if ((tnum <= (int16_t)(-12453)) ^ (INT_LEQUAL(tnum, (int16_t)(-12453)))) {
            printf("%s Failed test\n^6\n", testName);
            break;   
        }
        testNum += 1;
    }


    // EXIT ...
    return 0;
}


