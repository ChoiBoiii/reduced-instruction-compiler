/* 

    DESCRIPTION
    A header only library allowing code that can be compiled to all NAND's. Why? why not :)
    
    HOW TO CONFIGURE
    1. Set the data type of the register that will be used by defining OVERRIDE_REGISTER_TYPE before including header. 
       Using a uint variant is recommended. Size must not exceed 1024 bits. If OVERRIDE_REGISTER_TYPE is not defined, a 
       default value will be selected. Likely uint64, but not gauranteed.
    2. If you defined OVERRIDE_REGISTER_TYPE you must also define OVERRIDE_REGISTER_SIZE with the size, in bits, of the 
       newly specified register data type.
       WARNING: Do not wrap number in parentheses as this will break the precompiler parsing.

    NOTE
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

// Standard
// None 

// Local
#define BOOST_PP_LIMIT_REPEAT (1024) // Configure boost header to allow up to 1024 repeats
#include "boost/preprocessor/repetition/repeat.hpp"


// CONFIG ...

// Provide default values if not given override. Throw error in error cases.

// Register / Instruction word size info
typedef u_int16_t reg_t;      // The type to use to store the value of a register.
#define REGISTER_SIZE_BITS 16 // The number of bits in the register. Used to configure inctruction macros.

// AUTOGENERATE MACROS FOR PREPROCESSOR LOGIC

// // Determine the size of the register, minus 1
// #if REGISTER_SIZE_BITS == 8
//     #define REG_SIZE_SUB_1 7
// #elif REGISTER_SIZE_BITS == 16
//     #define REG_SIZE_SUB_1 15
// #elif REGISTER_SIZE_BITS == 32
//     #define REG_SIZE_SUB_1 31
// #elif REGISTER_SIZE_BITS == 64
//     #define REG_SIZE_SUB_1 63
// #elif REGISTER_SIZE_BITS == 128
//     #define REG_SIZE_SUB_1 127
// #elif REGISTER_SIZE_BITS == 256
//     #define REG_SIZE_SUB_1 255
// #elif REGISTER_SIZE_BITS == 512
//     #define REG_SIZE_SUB_1 511
// #elif REGISTER_SIZE_BITS == 1024
//     #define REG_SIZE_SUB_1 1023
// #endif
// #ifndef REG_SIZE_SUB_1
//     #error ERROR: Given register size isn't defined. Add to macro definitions.
// #endif


// HELPER MACROS ...

// Allow preprocessor to repeat lines n times
#define Fold(z, n, text)  text                       // Helper function for STRREP
#define STRREP(str, n) BOOST_PP_REPEAT(n, Fold, str) // Call 'STRREP' to repeat 'str' 'n' times


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

// Sets X to 0b1 if X contains any 1's, otherwise sets X to 0b0
#define FOLD_BITS_TO_1(X) ({   \
    X = OR(X, BSR(X, 32));     \
    X = OR(X, BSR(X, 16));     \
    X = OR(X, BSR(X, 8));      \
    X = OR(X, BSR(X, 4));      \
    X = OR(X, BSR(X, 2));      \
    X = OR(X, BSR(X, 1));      \
    X = AND(X, 1);             \
})

// Returns 1 if X is equal to zero
#define EQUAL0(X) ({           \
    reg_t v = X;               \
    FOLD_BITS_TO_1(v);         \
    v = XOR(v, 1);             \
    v;                         \
})    

// Returns 1 if X is not equal to zero
#define NEQUAL0(X) ({          \
    reg_t v = X;               \
    FOLD_BITS_TO_1(v);         \
    v;                         \
})

// Returns 1 if X and Y are not equal
#define NEQUAL(X, Y) ({        \
    reg_t v = XOR(X, Y);       \
    FOLD_BITS_TO_1(v);         \
    v;                         \
})

// Returns 1 if X and Y are equal
#define EQUAL(X, Y) ({         \
    reg_t v = XOR(X, Y);       \
    FOLD_BITS_TO_1(v);         \
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
});
#define UINT_ADD(X, Y) ({                              \
    reg_t tmp, keep, res;                              \
    keep = BSL(AND(X, Y), 1);                          \
    res = XOR(X, Y);                                   \
    STRREP(UINT_ADD_HELPER(tmp, keep, res),            \
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
