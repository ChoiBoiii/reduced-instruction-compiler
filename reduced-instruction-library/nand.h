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
    * Allows compilation with any emulated register size within {size : size = 2^n}, up to and including 1024 bits, as long as
      the host machine has a defined type of that size.
    * If an instruction is tagged with 'Register Size Dependant' in the table below its implementation is dependant on register 
      size and must be updated if the specified register size (currently 64-bit) is changed.
    * WARNING: be careful of implicit type promotion. 
      E.G., if two variables of reg_t are added using normal addition operator (a + b), the result may be implicitly upgraded to a
      larger int type (implementation defined). This may cause hard to find issues. For example:
      if 'reg_t' is 'u_int16_t'...
      the return of 'UINT_ADD(X, Y)' would be of type 'u_int16_t', BUT
      the return of 'X + Y' could be of type 'u_int32_t' due to implicit conversion.

    DEFINED INSTRUCTION SET
    |--------------------------------------------------------------------------------------------------------------------------
    | Instruction   | Description                     | Equivalent Operator   | Returns  | Register Size Dependant | Has Test |
    |--------------------------------------------------------------------------------------------------------------|-----------
    | NAND          | NAND Bitwise operator           | f(a, b) -> ~(a & b)   | Bitfield | F                       | T        |
    | BSL           | Bitshift left operator          | f(a, n) -> (a << n)   | Bitfield | F                       | T        |
    | BSR           | Bitshift right operator         | f(a, n) -> (a >> n)   | Bitfield | F                       | T        |
    | AND           | AND bitwise operator            | f(a, b) -> (a & b)    | Bitfield | F                       | T        |
    | OR            | OR bitwise operator             | f(a, b) -> (a | b)    | Bitfield | F                       | T        |
    | NOT           | NOT bitwise operator            | f(a)    -> (~a)       | Bitfield | F                       | T        |
    | XOR           | XOR bitwise operator            | f(a, b) -> (a ^ b)    | Bitfield | F                       | T        |
    | NEQUAL        | Bitwise inequality              | f(a, b) -> (a != b)   | 0 or 1   | T                       | T        |
    | NEQUAL0       | Bitwise inequality with zero    | f(a)    -> (a != 0)   | 0 or 1   | T                       | T        |
    | EQUAL         | Bitwise equality                | f(a, b) -> (a == b)   | 0 or 1   | T                       | T        |
    | EQUAL0        | Bitwise equality with zero      | f(a)    -> (a == 0)   | 0 or 1   | T                       | T        |
    | GREATER_THAN  | Bitwise greater than            | f(a, b) -> (a > b)    | 0 or 1   |                         |          |
    | GEQUAL        | Bitwise greater or equal to     | f(a, b) -> (a >= b)   | 0 or 1   |                         |          |
    | LESS_THAN     | Bitwise less than               | f(a, b) -> (a < b)    | 0 or 1   |                         |          |
    | LEQUAL        | Bitwise less than or equal to   | f(a, b) -> (a <= b)   | 0 or 1   |                         |          |
    | UINT_ADD      | Add two unsigned integers       | f(a, b) -> (a + b)    | n        | F                       | T        |
    | UINT_SUB      | Subtract two unsigned integers  | f(a, b) -> (a - b)    | n        |                         |          |
    | UINT_MULT     | Unsigned integer multiplication | f(a, b) -> (a * b)    | n        |                         |          |
    | UINT_DIV      | Unsigned integer division       | f(a, b) -> int(a / b) | n        |                         |          |
    | INT_ADD       | Signed integer addition         | f(a, b) -> (a + b)    | n        | F                       | T        |
    | INT_MULT      | Signed integer multiplication   | f(a, b) -> (a * b)    | n        |                         |          |
    | INT_DIV       | Signed integer division         | f(a, b) -> int(a / b) | n        |                         |          |
    |--------------------------------------------------------------------------------------------------------------------------

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


// EQUALITY OPERATORS ...

// Equivalent to (X != 0). Sets X to 1 if X contains any ones, else 0.
#define FOLD_ONCE_HELPER(X, S) \
    X = OR(X, BSR(X, S));
#define FOLD_ONCE_PARAMS_HELPER(Z, N, X) \
    FOLD_ONCE_HELPER(X, (HELPER_STRREP(2*, BOOST_PP_SUB(BOOST_PP_SUB(REGISTER_SIZE_BITS_LOG2, 1), N))1))
#define FOLD_BITS_TO_1(X, S) BOOST_PP_REPEAT(S, FOLD_ONCE_PARAMS_HELPER, X) X = AND(X, 1);

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

// Returns 1 if X is equal to zero
#define EQUAL0(X) ({           \
    reg_t v = X;               \
    FOLD_BITS_TO_1(v, REGISTER_SIZE_BITS_LOG2);         \
    v = XOR(v, 1);             \
    v;                         \
})    

// Returns 1 if X is not equal to zero
#define NEQUAL0(X) ({          \
    reg_t v = X;               \
    FOLD_BITS_TO_1(v, REGISTER_SIZE_BITS_LOG2);         \
    v;                         \
})

// Returns 1 if X and Y are not equal
#define NEQUAL(X, Y) ({        \
    reg_t v = XOR(X, Y);       \
    FOLD_BITS_TO_1(v, REGISTER_SIZE_BITS_LOG2);         \
    v;                         \
})

// Returns 1 if X and Y are equal
#define EQUAL(X, Y) ({         \
    reg_t v = XOR(X, Y);       \
    FOLD_BITS_TO_1(v, REGISTER_SIZE_BITS_LOG2);         \
    v = XOR(v, 1);             \
})

// Returns 1 if X > Y
#define GREATER_THAN(X, Y) ({  \
})

// Returns 1 if X >= Y
#define GEQUAL(X, Y)

// Returns 1 if X < Y
#define LESS_THAN(X, Y)

// Returns 1 if X <= Y
#define LEQUAL(X, Y)


// ARITHMETIC OPERATORS ...

// Inverts the sign of the given int
#define INT_SIGN_INVERT(X) ({})

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

// Unsigned integer subtraction of X-Y
#define UINT_SUB(X, Y) ({})

// Unsigned integer multiplication of X*Y
#define UINT_MULT(X, Y) ({})

// Unsigned integer division of X/Y
#define UINT_DIV(X, Y) ({})

// Signed integer addition of X+Y
#define INT_ADD(X, Y) ({               \
    UINT_ADD(X, Y);                    \
})

// Signed integer subtraction of X-Y
#define INT_SUB(X, Y) ({})

// Signed integer multiplication of X*Y
#define INT_MULT(X, Y) ({})

// Signed integer division of X/Y
#define INT_DIV(X, Y) ({})









/* UINT_ADD / INT_ADD using loop for arbitrary size and optimisation
#define UINT_ADD(X, Y) ({              \
    reg_t tmp, keep, res;              \
    keep = BSL(AND(X, Y), 1);          \
    res = XOR(X, Y);                   \
    loop:                              \
        tmp = keep;                    \
        keep = BSL(AND(keep, res), 1); \
        res = XOR(tmp, res);           \
        if (NEQUAL(keep, 0))           \
            goto loop;                 \
    res;                               \
})
*/


// REGISTER_SIZE_BITS
// REGISTER_SIZE_BITS_LOG2
// EQUAL0(X)