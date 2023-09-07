
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats. Max allowed by header is 1024.
#include "boost/preprocessor/repetition/repeat.hpp"
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/arithmetic/mul.hpp"
#include "boost/preprocessor/arithmetic/mod.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"
#include "boost/preprocessor/arithmetic/dec.hpp"

#define Fold(z, n, text)  text                       // Helper function for STRREP
#define STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str) // Call 'STRREP' to repeat 'str' 'n' times


#define REGSIZE 64

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


#define FOLD_ONCE(X, S) ({   \
    X = OR(X, BSR(X, S));    \
})

STRREP(UINT_ADD_HELPER(tmp, keep, res), REGISTER_SIZE_BITS); 
    

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



// Fold integer
// Equivalent to (X != 0). Sets X to 1 if X contains any ones, else 0.
#define FOLD_BITS_TO_1(X) ({   \
    X = OR(X, BSR(X, 32));     \
    X = OR(X, BSR(X, 16));     \
    X = OR(X, BSR(X, 8));      \
    X = OR(X, BSR(X, 4));      \
    X = OR(X, BSR(X, 2));      \
    X = OR(X, BSR(X, 1));      \
    X = AND(X, 1);             \
})
