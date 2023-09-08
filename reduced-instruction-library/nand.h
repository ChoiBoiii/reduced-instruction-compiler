/* 

    DESCRIPTION
    A header only library allowing code that can be compiled to all NAND's. Why? why not :)
    
    HOW TO CONFIGURE
    1. OPTIONAL: Set the data type of the register that will be used by defining RIC_OVERRIDE_REGISTER_TYPE before including header. 
       Using a uint variant is recommended. New register size must not exceed 1024 bits. If RIC_OVERRIDE_REGISTER_TYPE is not defined, 
       a default value will be selected automatically. Likely uint64, but not gauranteed.
    2. If you defined RIC_OVERRIDE_REGISTER_TYPE you must also define RIC_OVERRIDE_REGISTER_SIZE with the size, in bits, of the 
       newly specified register data type.
       WARNING: Do not wrap number in parentheses as this will break the precompiler parsing.

    NOTE
    * Allows emulated register sizes of (in bits) [8, 16, 32, 64, 128, 256, 512, 1024]
    * If an instruction is tagged with 'Register Size Dependant' in the table below its implementation is dependant on register 
      size and must be updated if the specified register size (currently 64-bit) is changed.
    * WARNING: be careful of implicit type promotion. 
      E.G., if two variables of reg_t are added using normal addition operator (a + b), the result may be implicitly upgraded to a
      larger int type (implementation defined). This may cause hard to find issues. For example:
      if 'reg_t' is 'u_int16_t'...
      the return of 'UINT_ADD(X, Y)' would be of type 'u_int16_t', BUT
      the return of 'X + Y' could be of type 'u_int32_t' due to implicit conversion.

    DEFINED INSTRUCTION SET
    |----------------------------------------------------------------------------------------------------------------------------
    | Instruction     | Description                     | Equivalent Operator   | Returns  | Register Size Dependant | Has Test |
    |----------------------------------------------------------------------------------------------------------------------------
    | NAND            | NAND Bitwise operator           | f(a, b) -> ~(a & b)   | Bitfield | F                       | T        |
    | BSL             | Bitshift left operator          | f(a, n) -> (a << n)   | Bitfield | F                       | T        |
    | BSR             | Bitshift right operator         | f(a, n) -> (a >> n)   | Bitfield | F                       | T        |
    | AND             | AND bitwise operator            | f(a, b) -> (a & b)    | Bitfield | F                       | T        |
    | OR              | OR bitwise operator             | f(a, b) -> (a | b)    | Bitfield | F                       | T        |
    | NOT             | NOT bitwise operator            | f(a)    -> (~a)       | Bitfield | F                       | T        |
    | XOR             | XOR bitwise operator            | f(a, b) -> (a ^ b)    | Bitfield | F                       | T        |
    | NEQUAL          | Bitwise inequality              | f(a, b) -> (a != b)   | 0 or 1   | F                       | T        |
    | NEQUAL0         | Bitwise inequality with zero    | f(a)    -> (a != 0)   | 0 or 1   | F                       | T        |
    | EQUAL           | Bitwise equality                | f(a, b) -> (a == b)   | 0 or 1   | F                       | T        |
    | EQUAL0          | Bitwise equality with zero      | f(a)    -> (a == 0)   | 0 or 1   | F                       | T        |
    | UINT_GTHAN      | Greater than between uints      | f(a, b) -> (a > b)    | 0 or 1   | F                       | T        |
    | UINT_GEQUAL     | Greater or equal between uints  | f(a, b) -> (a >= b)   | 0 or 1   | F                       | T        |
    | UINT_LTHAN      | Less than between uints         | f(a, b) -> (a < b)    | 0 or 1   | F                       | T        |
    | UINT_LEQUAL     | Less or equal between uints     | f(a, b) -> (a <= b)   | 0 or 1   | F                       | T        |
    | INT_GTHAN       | Greater than between ints       | f(a, b) -> (a > b)    | 0 or 1   |
    | INT_GEQUAL      | Greater or equal between ints   | f(a, b) -> (a >= b)   | 0 or 1   |
    | INT_LTHAN       | Less than between ints          | f(a, b) -> (a < b)    | 0 or 1   |
    | INT_LEQUAL      | Less or equal between ints      | f(a, b) -> (a <= b)   | 0 or 1   |
    | INT_SIGN_INVERT | Integer sign inversion          | f(a)    -> (-a)       | -n       | F                       | T        |
    | UINT_ADD        | Add two unsigned integers       | f(a, b) -> (a + b)    | n        | F                       | T        |
    | UINT_SUB        | Subtract two unsigned integers  | f(a, b) -> (a - b)    | n        | F                       | T        |
    | UINT_MULT       | Unsigned integer multiplication | f(a, b) -> (a * b)    | n        |
    | UINT_DIV        | Unsigned integer division       | f(a, b) -> int(a / b) | n        |
    | INT_ADD         | Signed integer addition         | f(a, b) -> (a + b)    | n        | F                       | T        |
    | INT_SUB         | Signed integer subtraction      | f(a, b) -> (a - b)    | n        | F                       | T        |
    | INT_MULT        | Signed integer multiplication   | f(a, b) -> (a * b)    | n        |
    | INT_DIV         | Signed integer division         | f(a, b) -> int(a / b) | n        |
    |----------------------------------------------------------------------------------------------------------------------------

    COMMENT TAGS
    * TODO     - A note of something that needs to be done
    * OPTIMISE - Associated code / function works but requires optimisation 

    TODO
    * Look into using preprocessor control flow and arithmetic for generalising operations
      to any register size automatically. E.G., repeat line n times. Insert n^2. etc. 

*/


// INCLUDE LIBRARIES ...

// Local
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats. Max allowed by header is 1024.
#include "boost/preprocessor/repetition/repeat.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"


// AUTOMATIC CONFIGURATION ...

// Throw error if override size is given but override type is not defined
#ifdef RIC_OVERRIDE_REGISTER_SIZE
    #ifndef RIC_OVERRIDE_REGISTER_TYPE
        #error Defined 'RIC_OVERRIDE_REGISTER_SIZE' but did not define 'RIC_OVERRIDE_REGISTER_TYPE'. Please define 'RIC_OVERRIDE_REGISTER_TYPE'.
    #endif
#endif

// Throw error if override type is given but override size is not defined
#ifdef RIC_OVERRIDE_REGISTER_TYPE
    #ifndef RIC_OVERRIDE_REGISTER_SIZE
        #error Defined 'RIC_OVERRIDE_REGISTER_TYPE' but did not define 'RIC_OVERRIDE_REGISTER_SIZE'. Please define 'RIC_OVERRIDE_REGISTER_SIZE'.
    #endif
#endif

// Determine use of override or default values
#if defined(RIC_OVERRIDE_REGISTER_TYPE) && defined(RIC_OVERRIDE_REGISTER_SIZE)
    #define RIC_TMP_CONFIG_REGISTER_TYPE RIC_OVERRIDE_REGISTER_TYPE
    #define RIC_TMP_CONFIG_REG_SIZE_BITS RIC_OVERRIDE_REGISTER_SIZE
#else
    #define RIC_TMP_CONFIG_REGISTER_TYPE u_int64_t
    #define RIC_TMP_CONFIG_REG_SIZE_BITS 64
#endif

// Register / Instruction word size info
typedef RIC_TMP_CONFIG_REGISTER_TYPE reg_t;             // The type to use to store the value of a register.
#define REGISTER_SIZE_BITS RIC_TMP_CONFIG_REG_SIZE_BITS // The number of bits in the register. Used to configure inctruction macros.

// Determine log2(size) of specified register
#if REGISTER_SIZE_BITS == 8
    #define REGISTER_SIZE_BITS_LOG2 3
#elif REGISTER_SIZE_BITS == 16
    #define REGISTER_SIZE_BITS_LOG2 4
#elif REGISTER_SIZE_BITS == 32
    #define REGISTER_SIZE_BITS_LOG2 5
#elif REGISTER_SIZE_BITS == 64
    #define REGISTER_SIZE_BITS_LOG2 6
#elif REGISTER_SIZE_BITS == 128
    #define REGISTER_SIZE_BITS_LOG2 7
#elif REGISTER_SIZE_BITS == 256
    #define REGISTER_SIZE_BITS_LOG2 8
#elif REGISTER_SIZE_BITS == 512
    #define REGISTER_SIZE_BITS_LOG2 9
#elif REGISTER_SIZE_BITS == 1024
    #define REGISTER_SIZE_BITS_LOG2 10
#else
    #ifdef REGISTER_SIZE_BITS
        #error REGISTER_SIZE_BITS is defined to an invalid value. Must be a power of two and within the range (inclusive) [8, 1024].
    #else
        #error REGISTER_SIZE_BITS is not defined. Please define to a power of 2, within the range (inclusive) [8, 1024].
    #endif
#endif


// HELPER MACROS ...

// Function to allow repeat of given repeat lines
#define HELPER_FOLD_STR(Z, N, T)  T                                // Helper function for HELPER_STRREP
#define HELPER_STRREP(S, N) BOOST_PP_REPEAT(N, HELPER_FOLD_STR, S) // Call 'HELPER_STRREP' to repeat 'S' 'N' times


// BITWISE OPERATORS ...

// Base operators (not NAND based)
#define NAND(X, Y)  ( ~(X & Y)                     ) 
#define BSL(X, N)   ( (X << N)                     ) 
#define BSR(X, N)   ( (X >> N)                     ) 

// Other decomposed bitwise operators
#define AND(X, Y)   ( NAND(NAND(X, Y), NAND(X, Y)) ) 
#define OR(X, Y)    ( NAND(NAND(X, X), NAND(Y, Y)) ) 
#define NOT(X)      ( NAND(X, X)                   ) 
#define XOR(X, Y)   ( AND(OR(X, Y), NAND(X, Y))    ) 


// ARITHMETIC OPERATORS ...

// Unsigned integer addition of X+Y
#define UINT_ADD_HELPER(T, K, R) ({                    \
    T = K;                                             \
    K = BSL(AND(K, R), 1);                             \
    R = XOR(T, R);                                     \
})
#define UINT_ADD(X, Y) ({                              \
    reg_t tmp, keep, res;                              \
    keep = BSL(AND(X, Y), 1);                          \
    res = XOR(X, Y);                                   \
    HELPER_STRREP(UINT_ADD_HELPER(tmp, keep, res);,    \
        REGISTER_SIZE_BITS);                           \
    res;                                               \
})

// Inverts the sign of the given int using two's compliment: invert then add 1
#define INT_SIGN_INVERT(X) ({                          \
    UINT_ADD(NOT(X), 1);                               \
})                                                  

// Signed integer addition of X+Y
#define INT_ADD(X, Y) ({                               \
    UINT_ADD(X, Y);                                    \
})

// Unsigned integer subtraction of X-Y
#define UINT_SUB(X, Y) ({                              \
    INT_ADD(X, INT_SIGN_INVERT(Y));                    \
})

// Signed integer subtraction of X-Y
#define INT_SUB(X, Y) ({                               \
    UINT_SUB(X, Y);                                    \
})

// Unsigned integer multiplication of X*Y
#define UINT_MULT(X, Y) ({})

// Unsigned integer division of X/Y
#define UINT_DIV(X, Y) ({})


// EQUALITY OPERATORS ...

// HELPER: Returns a formatted fold line for the FOLD_BITS_TO_1_EQ_HELPER method
#define FOLD_ONCE_HELPER_(X, S)                          \
    (X = OR(X, BSR(X, S)));

// HELPER: Returns the ammount of bitshift required for an iteration of the FOLD_BITS_TO_1_EQ_HELPER method
#define FOLD_ONCE_GET_SHIFT_HELPER_(N)                   \
    (HELPER_STRREP(2*, BOOST_PP_SUB(                     \
        BOOST_PP_SUB(REGISTER_SIZE_BITS_LOG2, 1), N)) 1) \

// HELPER: Returns a fully formatted fold line for the FOLD_BITS_TO_1_EQ_HELPER method
#define FOLD_ONCE_PARAMS_HELPER_(Z, N, X) ({             \
    FOLD_ONCE_HELPER_(X, FOLD_ONCE_GET_SHIFT_HELPER_(N)) \
});

// HELPER: Equivalent to (X != 0). Sets X to 1 if X contains any ones, else 0.
#define FOLD_BITS_TO_1_EQ_HELPER(X, S) ({                \
    BOOST_PP_REPEAT(S, FOLD_ONCE_PARAMS_HELPER_, X);     \
    X = AND(X, 1);                                       \
})

// HELPER: Returns a formatted fold line for the EXTRACT_MSB_EQ_HELPER method
#define EXTRACT_MSB_EQ_FOLD_ONCE_HELPER_(X, S)           \
    (X = OR(X, BSR(X, S)));

// HELPER: Returns the ammount of bitshift required for an iteration of the EXTRACT_MSB_EQ_HELPER method
#define EXTRACT_MSB_EQ_GET_SHIFT_HELPER_(N)              \
    (HELPER_STRREP(2*, N) 1)                             \

// HELPER: Returns a fully formatted fold line for the EXTRACT_MSB_EQ_HELPER method
#define EXTRACT_MSB_EQ_FOLD_PARAMS_HELPER_(Z, N, X) ({   \
    EXTRACT_MSB_EQ_FOLD_ONCE_HELPER_(                    \
        X, EXTRACT_MSB_EQ_GET_SHIFT_HELPER_(N))          \
});

// HELPER: Used to help extract most significant bit in some equivalence instruction methods
#define EXTRACT_MSB_EQ_HELPER(X, S) ({                   \
    BOOST_PP_REPEAT(                                     \
        S, EXTRACT_MSB_EQ_FOLD_PARAMS_HELPER_, X);       \
})

// Returns 1 if X is equal to zero
#define EQUAL0(X) ({               \
    reg_t v = X;                   \
    FOLD_BITS_TO_1_EQ_HELPER(v,    \
        REGISTER_SIZE_BITS_LOG2);  \
    v = XOR(v, 1);                 \
    v;                             \
})    

// Returns 1 if X is not equal to zero
#define NEQUAL0(X) ({              \
    reg_t v = X;                   \
    FOLD_BITS_TO_1_EQ_HELPER(v,    \
        REGISTER_SIZE_BITS_LOG2);  \
    v;                             \
})

// Returns 1 if X and Y are equal
#define EQUAL(X, Y) ({             \
    reg_t v = XOR(X, Y);           \
    FOLD_BITS_TO_1_EQ_HELPER(v,    \
        REGISTER_SIZE_BITS_LOG2);  \
    v = XOR(v, 1);                 \
})

// Returns 1 if X and Y are not equal
#define NEQUAL(X, Y) ({            \
    reg_t v = XOR(X, Y);           \
    FOLD_BITS_TO_1_EQ_HELPER(v,    \
        REGISTER_SIZE_BITS_LOG2);  \
    v;                             \
})

// Returns 1 if X > Y {OPTIMISE}
#define UINT_GTHAN(X, Y) ({        \
    reg_t v = X ^ Y;               \
    EXTRACT_MSB_EQ_HELPER(v,       \
        REGISTER_SIZE_BITS_LOG2);  \
    v = UINT_SUB(v, BSR(v, 1));    \
    v = XOR(AND(Y, v), v);         \
    FOLD_BITS_TO_1_EQ_HELPER(v,    \
        REGISTER_SIZE_BITS_LOG2);  \
    v;                             \
})

// Returns 1 if X >= Y {OPTIMISE}
#define UINT_GEQUAL(X, Y) ({       \
    OR(                            \
        UINT_GTHAN(X,Y),           \
        EQUAL(X,Y)                 \
    );                             \
})

// Returns 1 if X < Y {OPTIMISE}
#define UINT_LTHAN(X, Y) ({        \
    XOR(UINT_GEQUAL(X, Y), 1);     \
})

// Returns 1 if X <= Y {OPTIMISE} 
#define UINT_LEQUAL(X, Y) ({       \
    XOR(UINT_GTHAN(X, Y), 1);      \
})

// Returns 1 if X > Y {OPTIMISE}
#define INT_GTHAN(X, Y) ({         \
    UINT_GTHAN(X, Y);              \
})


