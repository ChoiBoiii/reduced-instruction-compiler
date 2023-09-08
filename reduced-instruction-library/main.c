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
typedef int16_t signed_reg_type;

// Tests
bool unit_test_BW_NAND(reg_t n1, reg_t n2)         {return ((reg_t)~(n1 & n2)) ^ (reg_t)(BW_NAND(n1, n2));}
bool unit_test_BW_BSL(reg_t n1, reg_t n2)          {return ((reg_t)(n1 << n2) ^ (reg_t)(BW_BSL(n1, n2)));}
bool unit_test_BW_BSR(reg_t n1, reg_t n2)          {return ((reg_t)(n1 >> n2) ^ (BW_BSR(n1, n2)));}
bool unit_test_BW_AND(reg_t n1, reg_t n2)          {return ((reg_t)(n1 & n2) ^ (BW_AND(n1, n2)));}
bool unit_test_BW_OR(reg_t n1, reg_t n2)           {return ((reg_t)(n1 | n2) ^ (BW_OR(n1, n2)));}
bool unit_test_BW_NOT(reg_t n1, reg_t n2)          {return ((reg_t)(~n1) ^ (reg_t)(BW_NOT(n1)));}
bool unit_test_BW_XOR(reg_t n1, reg_t n2)          {return ((reg_t)(n1 ^ n2) ^ (BW_XOR(n1, n2)));}
bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)        {return ((reg_t)(n1 == n2) ^ (BW_EQUAL(n1, n2)));}
bool unit_test_BW_EQUAL0(reg_t n1, reg_t n2)       {return ((reg_t)(n1 == 0) ^ (BW_EQUAL0(n1)));}
bool unit_test_BW_NEQUAL0(reg_t n1, reg_t n2)      {return ((reg_t)(n1 != 0) ^ (BW_NEQUAL0(n1)));}
bool unit_test_BW_NEQUAL(reg_t n1, reg_t n2)       {return ((reg_t)(n1 != n2) ^ (BW_NEQUAL(n1, n2)));}
bool unit_test_UINT_ADD(reg_t n1, reg_t n2)        {return ((reg_t)(n1 + n2) ^ (UINT_ADD(n1, n2)));}
bool unit_test_INT_ADD(reg_t n1, reg_t n2)         {return ((reg_t)((signed_reg_type)n1 + (signed_reg_type)n2) ^ (INT_ADD(n1, n2)));}
bool unit_test_INT_SIGN_INVERT(reg_t n1, reg_t n2) {return ((reg_t)(-n1) ^ (INT_SIGN_INVERT(n1)));}
bool unit_test_UINT_SUB(reg_t n1, reg_t n2)        {return ((reg_t)(n1 - n2) ^ (UINT_SUB(n1, n2)));}
bool unit_test_INT_SUB(reg_t n1, reg_t n2)         {return ((reg_t)((signed_reg_type)n1 - (signed_reg_type)n2) ^ (INT_SUB(n1, n2)));}
bool unit_test_UINT_GTHAN(reg_t n1, reg_t n2)      {return ((reg_t)(n1 > n2) ^ (UINT_GTHAN(n1, n2)));}
bool unit_test_UINT_GEQUAL(reg_t n1, reg_t n2)     {return ((reg_t)(n1 >= n2) ^ (UINT_GEQUAL(n1, n2)));}
bool unit_test_UINT_LTHAN(reg_t n1, reg_t n2)      {return ((reg_t)(n1 < n2) ^ (UINT_LTHAN(n1, n2)));}
bool unit_test_UINT_LEQUAL(reg_t n1, reg_t n2)     {return ((reg_t)(n1 <= n2) ^ (UINT_LEQUAL(n1, n2)));}
// bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)        {return ((reg_t)(n1 == n2) ^ (BW_EQUAL(n1, n2)));}
// bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)        {return ((reg_t)(n1 == n2) ^ (BW_EQUAL(n1, n2)));}
// bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)        {return ((reg_t)(n1 == n2) ^ (BW_EQUAL(n1, n2)));}
// bool unit_test_BW_EQUAL(reg_t n1, reg_t n2)        {return ((reg_t)(n1 == n2) ^ (BW_EQUAL(n1, n2)));}


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

    // BW_EQUAL0
    run_test("BW_EQUAL0", unit_test_BW_EQUAL0);

    // BW_NEQUAL0
    run_test("BW_NEQUAL0", unit_test_BW_NEQUAL0);

    // BW_NEQUAL
    run_test("BW_NEQUAL", unit_test_BW_NEQUAL);

    // BW_EQUAL
    run_test("BW_EQUAL", unit_test_BW_EQUAL);

    // UINT_ADD
    run_test("UINT_ADD", unit_test_UINT_ADD);

    // INT_ADD
    run_test("INT_ADD", unit_test_INT_ADD);

    // INT_SIGN_INVERT
    run_test("INT_SIGN_INVERT", unit_test_INT_SIGN_INVERT);

    // UINT_SUB
    run_test("UINT_SUB", unit_test_UINT_SUB);

    // INT_SUB
    run_test("INT_SUB", unit_test_INT_SUB);

    // UINT_GTHAN
    run_test("UINT_GTHAN", unit_test_UINT_GTHAN);

    // UINT_GEQUAL
    run_test("UINT_GEQUAL", unit_test_UINT_GEQUAL);

    // UINT_LTHAN
    run_test("UINT_LTHAN", unit_test_UINT_LTHAN);

    // UINT_LEQUAL
    run_test("UINT_LEQUAL", unit_test_UINT_LEQUAL);

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


