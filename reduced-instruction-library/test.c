#include <stdio.h>
#include <boost/preprocessor/repetition/repeat.hpp>


// Base operators (not NAND based)
// #define NAND(X, Y)    ( ~(X & Y)                     ) 
// #define BSL(X, N)     ( (X << N)                     ) 
// #define BSR(X, N)     ( (X >> N)                     ) 

// // Other decomposed bitwise operators
// #define AND(X, Y)     ( NAND(NAND(X, Y), NAND(X, Y)) ) 
// #define OR(X, Y)      ( NAND(NAND(X, X), NAND(Y, Y)) ) 
// #define NOT(X)        ( NAND(X, X)                   ) 
// #define XOR(X, Y)     ( AND(OR(X, Y), NAND(X, Y))    ) 

#define UINT_ADD_HELPER(Z, N) ({       \
    tmp = keep;                        \
    keep = BSL(AND(keep, res), 1);     \
    res = XOR(tmp, res);               \
})


int main(){

    int res;
    int keep;
    int tmp;

    BOOST_PP_REPEAT(2, UINT_ADD_HELPER(X,Y), );

    return 0;
}