
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats. Max allowed by header is 1024.
#include "boost/preprocessor/repetition/repeat.hpp"
#include "boost/preprocessor/arithmetic/add.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/arithmetic/mul.hpp"
#include "boost/preprocessor/arithmetic/mod.hpp"
#include "boost/preprocessor/arithmetic/div.hpp"
#include "boost/preprocessor/arithmetic/dec.hpp"






#define REGSIZE 16

// Get the log2(size) of the register type
#if REGSIZE == 8
    #define REGISTER_SIZE_BITS_LOG2 3
#elif REGSIZE == 16
    #define REGISTER_SIZE_BITS_LOG2 4
#elif REGSIZE == 32
    #define REGISTER_SIZE_BITS_LOG2 5
#elif REGSIZE == 64
    #define REGISTER_SIZE_BITS_LOG2 6
#elif REGSIZE == 128
    #define REGISTER_SIZE_BITS_LOG2 7
#elif REGSIZE == 256
    #define REGISTER_SIZE_BITS_LOG2 8
#elif REGSIZE == 512
    #define REGISTER_SIZE_BITS_LOG2 9
#elif REGSIZE == 1024
    #define REGISTER_SIZE_BITS_LOG2 10
#else
    #ifdef REGSIZE
        #error REGSIZE is defined to an invalid value. Must be a power of two and within the range (inclusive) [8, 1024].
    #else
        #error REGSIZE is not defined. Please define to a power of 2, within the range (inclusive) [8, 1024].
    #endif
#endif



// #define Fold(z, n, text)  text                               // Helper function for STRREP
// #define HELPER_STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str)  // Call 'STRREP' to repeat 'str' 'n' times
// #define FOLD_ONCE_HELPER(X, S) (X = OR(X, BSR(X, S)));
// #define FOLD_ONCE_PARAMS_HELPER(z, n, X) FOLD_ONCE_HELPER( X, ( HELPER_STRREP( 2* , BOOST_PP_SUB(z, n) ) 1 ) )
// #define FOLD_SIZE_LOG2(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE_PARAMS_HELPER, X)

// #define Fold(z, n, text)  text                               // Helper function for STRREP
// #define HELPER_STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str)  // Call 'STRREP' to repeat 'str' 'n' times
// #define FOLD_ONCE_HELPER(X, S) (X = OR(X, BSR(X, S)));
// #define FOLD_ONCE_PARAMS_HELPER(z, n, X) FOLD_ONCE_HELPER( X, ( HELPER_STRREP( 2* , BOOST_PP_SUB(BOOST_PP_SUB(REGISTER_SIZE_BITS_LOG2, 1), n) ) 1 ) )
// #define FOLD_SIZE_LOG2(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE_PARAMS_HELPER, X)


#define Fold(z, n, text)  text                               // Helper function for STRREP
#define HELPER_STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str)  // Call 'STRREP' to repeat 'str' 'n' times
#define FOLD_ONCE_HELPER(X, S) (X = OR(X, BSR(X, S)));
#define FOLD_ONCE_PARAMS_HELPER(z, n, X) FOLD_ONCE_HELPER( X, ( HELPER_STRREP( 2* , BOOST_PP_SUB(BOOST_PP_SUB(REGISTER_SIZE_BITS_LOG2, 1), n) ) 1 ) )
#define FOLD_SIZE_LOG2(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE_PARAMS_HELPER, X)


// Returns 1 if X is equal to zero
#define EQUAL0(X) ({           \
    reg_t v = X;               \
    FOLD_SIZE_LOG2(v, REGISTER_SIZE_BITS_LOG2);         \
    v = XOR(v, 1);             \
    v;                         \
})    

EQUAL0(X)








// #define Fold(z, n, text)  text                       // Helper function for STRREP
// #define STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str) // Call 'STRREP' to repeat 'str' 'n' times
// #define FOLD(X, S) (X = OR(X, BSR(X, S)));
// #define FOLD_ONCE(z, n, X) FOLD(X, 1 STRREP(*2, BOOST_PP_SUB(z, n)))
// #define FOLD_SIZE_LOG2(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE, X)