
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats. Max allowed by header is 1024.
#include "boost/preprocessor/repetition/repeat.hpp"
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/arithmetic/mul.hpp"
#include "boost/preprocessor/arithmetic/mod.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"
#include "boost/preprocessor/arithmetic/dec.hpp"






#define REGSIZE 8

// Get the log2(size) of the register type
#if REGSIZE == 8
    #define REGSIZE_LOG_2 3
#elif REGSIZE == 16
    #define REGSIZE_LOG_2 4
#elif REGSIZE == 32
    #define REGSIZE_LOG_2 5
#elif REGSIZE == 64
    #define REGSIZE_LOG_2 6
#elif REGSIZE == 128
    #define REGSIZE_LOG_2 7
#elif REGSIZE == 256
    #define REGSIZE_LOG_2 8
#elif REGSIZE == 512
    #define REGSIZE_LOG_2 9
#elif REGSIZE == 1024
    #define REGSIZE_LOG_2 10
#else
    #ifdef REGSIZE
        #error REGSIZE is defined to an invalid value. Must be a power of two and within the range (inclusive) [8, 1024].
    #else
        #error REGSIZE is not defined. Please define to a power of 2, within the range (inclusive) [8, 1024].
    #endif
#endif


#define FOLD(X, S) (X = OR(X, BSR(X, S)));
#define FOLD_ONCE(z, n, X) FOLD(X, BOOST_PP_SUB(z, n))
#define FOLD_SIZE_LOG2(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE, X)

FOLD_SIZE_LOG2(v, 3)


1
1 * 2
1 * 2 * 2
1 * 2 * 2 * 2


// #define Fold(z, n, text)  z n text                       // Helper function for STRREP

// #define STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str) // Call 'STRREP' to repeat 'str' 'n' times


// #define FOLD_ONCE(X, S) (X = OR(X, BSR(X, S)));

// #define TEST2(z, n, text) text z n
// #define TEST(X, S) BOOST_PP_REPEAT(S, TEST2, X)

// TEST(var, 3)






// z = iter size
// n = current iter

// #define FOLD_SIZE_LOG2(X, S) ({

// })
    

// // Fold integer
// // Equivalent to (X != 0). Sets X to 1 if X contains any ones, else 0.
// #define FOLD_BITS_TO_1(X) ({   \
//     X = OR(X, BSR(X, 32));     \
//     X = OR(X, BSR(X, 16));     \
//     X = OR(X, BSR(X, 8));      \
//     X = OR(X, BSR(X, 4));      \
//     X = OR(X, BSR(X, 2));      \
//     X = OR(X, BSR(X, 1));      \
//     X = AND(X, 1);             \
// })



// // Fold integer
// // Equivalent to (X != 0). Sets X to 1 if X contains any ones, else 0.
// #define FOLD_BITS_TO_1(X) ({   \
//     X = OR(X, BSR(X, 32));     \
//     X = OR(X, BSR(X, 16));     \
//     X = OR(X, BSR(X, 8));      \
//     X = OR(X, BSR(X, 4));      \
//     X = OR(X, BSR(X, 2));      \
//     X = OR(X, BSR(X, 1));      \
//     X = AND(X, 1);             \
// })
