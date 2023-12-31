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
    * Emulates two's compliment integers
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
    | BW_NAND         | Bitwise NAND operator           | f(a, b) -> ~(a & b)   | Bitfield | F                       | T        |
    | BW_BSL          | Bitshift left operator          | f(a, n) -> (a << n)   | Bitfield | F                       | T        |
    | BW_BSR          | Bitshift right operator         | f(a, n) -> (a >> n)   | Bitfield | F                       | T        |
    | BW_AND          | Bitwise AND operator            | f(a, b) -> (a & b)    | Bitfield | F                       | T        |
    | BW_OR           | Bitwise OR operator             | f(a, b) -> (a | b)    | Bitfield | F                       | T        |
    | BW_NOT          | Bitwise NOT operator            | f(a)    -> (~a)       | Bitfield | F                       | T        |
    | BW_XOR          | Bitwise XOR operator            | f(a, b) -> (a ^ b)    | Bitfield | F                       | T        |
    | BW_NEQUAL       | Bitwise inequality              | f(a, b) -> (a != b)   | 0 or 1   | F                       | T        |
    | BW_NEQUAL0      | Bitwise inequality with zero    | f(a)    -> (a != 0)   | 0 or 1   | F                       | T        |
    | BW_EQUAL        | Bitwise equality                | f(a, b) -> (a == b)   | 0 or 1   | F                       | T        |
    | BW_EQUAL0       | Bitwise equality with zero      | f(a)    -> (a == 0)   | 0 or 1   | F                       | T        |
    | UINT_GTHAN      | Greater than between uints      | f(a, b) -> (a > b)    | 0 or 1   | F                       | T        |
    | UINT_GEQUAL     | Greater or equal between uints  | f(a, b) -> (a >= b)   | 0 or 1   | F                       | T        |
    | UINT_LTHAN      | Less than between uints         | f(a, b) -> (a < b)    | 0 or 1   | F                       | T        |
    | UINT_LEQUAL     | Less or equal between uints     | f(a, b) -> (a <= b)   | 0 or 1   | F                       | T        |
    | INT_GTHAN       | Greater than between ints       | f(a, b) -> (a > b)    | 0 or 1   | F                       | T        |
    | INT_GEQUAL      | Greater or equal between ints   | f(a, b) -> (a >= b)   | 0 or 1   | F                       | T        |
    | INT_LTHAN       | Less than between ints          | f(a, b) -> (a < b)    | 0 or 1   | F                       | T        |
    | INT_LEQUAL      | Less or equal between ints      | f(a, b) -> (a <= b)   | 0 or 1   | F                       | T        |
    | INT_SIGN_INVERT | Integer sign inversion          | f(a)    -> (-a)       | -n       | F                       | T        |
    | UINT_ADD        | Add two unsigned integers       | f(a, b) -> (a + b)    | n        | F                       | T        |
    | UINT_SUB        | Subtract two unsigned integers  | f(a, b) -> (a - b)    | n        | F                       | T        |
    | UINT_MULT       | Unsigned integer multiplication | f(a, b) -> (a * b)    | n        | F                       | T        |
    | UINT_DIV        | Unsigned integer division       | f(a, b) -> int(a / b) | n        |
    | INT_ADD         | Signed integer addition         | f(a, b) -> (a + b)    | n        | F                       | T        |
    | INT_SUB         | Signed integer subtraction      | f(a, b) -> (a - b)    | n        | F                       | T        |
    | INT_MULT        | Signed integer multiplication   | f(a, b) -> (a * b)    | n        | F                       | T        |
    | INT_DIV         | Signed integer division         | f(a, b) -> int(a / b) | n        |
    |----------------------------------------------------------------------------------------------------------------------------

    COMMENT TAGS
    * TODO     - A note of something that needs to be done
    * OPTIMISE - Associated code / function works but requires optimisation 

    TODO
    * Look into using preprocessor control flow and arithmetic for generalising operations
      to any register size automatically. E.G., repeat line n times. Insert n^2. etc. 

*/


// HEADER GUARD ...
#ifndef REDUCED_INSTRUCTION_COMPILER_H
#define REDUCED_INSTRUCTION_COMPILER_H


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

// REPEAT GIVEN LINE X TIMES

// Helper function to filter inputs allowing folding in STRREP
#define STRREP_FOLD_HELPER_(_, N, T)  T

// Call 'STRREP' to repeat 'S' 'N' times
#define STRREP(S, N)                                \
    BOOST_PP_REPEAT(N, STRREP_FOLD_HELPER_, S)


// GET BITMASK 0b1111... IF X != 0 OR 0b0000... IF X == 0

// Helper function to generate a line for the bitmask generation in GENERATE_IFMASK
#define GENERATE_IFMASK_HELPER_(_, N, X)            \
    X |= (X << (STRREP(2*, N) 1));

// Returns a full 0xFFFF... bitmask if (X & 1) else 0x0000...
#define GENERATE_IFMASK(X) ({                       \
    const reg_t _ric_scope_clash_workaround = X;    \
    reg_t _ric_ifmask = BW_AND(                     \
        _ric_scope_clash_workaround, 1);            \
    BOOST_PP_REPEAT(REGISTER_SIZE_BITS_LOG2,        \
        GENERATE_IFMASK_HELPER_, _ric_ifmask);      \
    _ric_ifmask;                                    \
})


// GET 0b01 IF X != 0 ELSE 0b00

// Helper function to generate line for the bitfield folding in FOLD_BITS_TO_1
#define FOLD_BITS_TO_1_HELPER_(_, N, X)             \
    X = BW_OR(X, BW_BSR(X, (STRREP(2*,              \
        BOOST_PP_SUB(BOOST_PP_SUB(                  \
            REGISTER_SIZE_BITS_LOG2, 1), N)) 1)));  \

// Returns 0x01 if (X != 0) else 0x00
#define FOLD_BITS_TO_1(X) ({                        \
    const reg_t _ric_scope_clash_workaround = X;    \
    reg_t _ric_out = _ric_scope_clash_workaround;   \
    BOOST_PP_REPEAT(REGISTER_SIZE_BITS_LOG2,        \
        FOLD_BITS_TO_1_HELPER_, _ric_out);          \
    _ric_out = BW_AND(_ric_out, 1);                 \
    _ric_out;                                       \
})


// GENERATE BITMSK OF 1's FROM LEAST SIGNIFICANT BIT TO MSB IN GIVEN VALUE X

// Helper function to generate a line for MSB bitmask extraction in GENERATE_MASK_UP_TO_MSB
#define GENERATE_MASK_UP_TO_MSB_HELPER_(_, N, X)    \
    X = BW_OR(X, BW_BSR(X, (STRREP(2*, N) 1)));

// HELPER: Used to help extract most significant bit in some equivalence instruction methods
#define GENERATE_MASK_UP_TO_MSB(X) ({               \
    reg_t out = X;                                  \
    BOOST_PP_REPEAT(REGISTER_SIZE_BITS_LOG2,        \
        GENERATE_MASK_UP_TO_MSB_HELPER_, out);      \
    out;                                            \
})


// BITWISE OPERATORS ...

// Base operators (not NAND based)
#define BW_NAND(X, Y)  ( ~(X & Y) ) 
#define BW_BSL(X, N)   ( (X << N) ) 
#define BW_BSR(X, N)   ( (X >> N) ) 

// Other decomposed bitwise operators
#define BW_AND(X, Y)   ( BW_NAND(BW_NAND(X, Y), BW_NAND(X, Y)) ) 
#define BW_OR(X, Y)    ( BW_NAND(BW_NAND(X, X), BW_NAND(Y, Y)) ) 
#define BW_NOT(X)      ( BW_NAND(X, X)                         ) 
#define BW_XOR(X, Y)   ( BW_AND(BW_OR(X, Y), BW_NAND(X, Y))    ) 


// ARITHMETIC OPERATORS ...

// Universally adds signed and unsigned ints [int, uint]
#define SIGN_INDEPENDANT_INT_ADD_PERFORM_ADD_CYCLE_     \
    _ric_tmp = _ric_keep;                               \
    _ric_keep = BW_BSL(BW_AND(_ric_keep, _ric_res), 1); \
    _ric_res = BW_XOR(_ric_tmp, _ric_res);                                   
#define SIGN_INDEPENDANT_INT_ADD(X, Y) ({               \
    const reg_t _ric_scope_clash_workaround1 = X;       \
    const reg_t _ric_scope_clash_workaround2 = Y;       \
    reg_t _ric_tmp, _ric_keep, _ric_res;                \
    _ric_keep = BW_BSL(BW_AND(                          \
        _ric_scope_clash_workaround1,                   \
        _ric_scope_clash_workaround2), 1);              \
    _ric_res = BW_XOR(                                  \
        _ric_scope_clash_workaround1,                   \
        _ric_scope_clash_workaround2);                  \
    STRREP(SIGN_INDEPENDANT_INT_ADD_PERFORM_ADD_CYCLE_, \
        REGISTER_SIZE_BITS);                            \
    _ric_res;                                           \
})

// Inverts the sign of the given int using two's compliment by inverting then adding 1
#define INT_SIGN_INVERT(X) ({                           \
    SIGN_INDEPENDANT_INT_ADD(BW_NOT(X), 1);             \
})                                                  

// Unsigned integer addition of X+Y
#define UINT_ADD(X, Y) ({                               \
    SIGN_INDEPENDANT_INT_ADD(X, Y);                     \
})

// Signed integer addition of X+Y
#define INT_ADD(X, Y) ({                                \
    SIGN_INDEPENDANT_INT_ADD(X, Y);                     \
})

// Unsigned integer subtraction of X-Y
#define UINT_SUB(X, Y) ({                               \
    SIGN_INDEPENDANT_INT_ADD(X, INT_SIGN_INVERT(Y));    \
})

// Signed integer subtraction of X-Y
#define INT_SUB(X, Y) ({                                \
    SIGN_INDEPENDANT_INT_ADD(X, INT_SIGN_INVERT(Y));    \
})

// Universally multiplies signed and unsigned ints [int, uint]
#define SIGN_INDEPENDANT_INT_MULT_CYCLE_                \
    _ric_ifmask = GENERATE_IFMASK(_ric_b);              \
    _ric_result = INT_ADD(                              \
        _ric_result, BW_AND(_ric_a, _ric_ifmask));      \
    _ric_a = BW_BSL(_ric_a, 1);                         \
    _ric_b = BW_BSR(_ric_b, 1);                  
#define SIGN_INDEPENDANT_INT_MULT(X, Y) ({              \
    const reg_t _ric_scope_clash_workaround1 = X;       \
    const reg_t _ric_scope_clash_workaround2 = Y;       \
    reg_t _ric_ifmask;                                  \
    reg_t _ric_a = _ric_scope_clash_workaround1;        \
    reg_t _ric_b = _ric_scope_clash_workaround2;        \
    reg_t _ric_result = 0;                              \
    STRREP(SIGN_INDEPENDANT_INT_MULT_CYCLE_,            \
        REGISTER_SIZE_BITS);                            \
    _ric_result;                                        \
})

// Unsigned integer multiplication of X*Y
#define UINT_MULT(X, Y) ({                              \
    SIGN_INDEPENDANT_INT_MULT(X, Y);                    \
})

// Signed integer multiplication of X*Y
#define INT_MULT(X, Y) ({                               \
    SIGN_INDEPENDANT_INT_MULT(X, Y);                    \
})

// Unsigned integer division of X/Y
#define UINT_DIV(X, Y) ({                               \
})


// EQUALITY OPERATORS ...

// Returns 1 if X is equal to zero
#define BW_EQUAL0(X) ({                                 \
    const reg_t _ric_scope_clash_workaround = X;        \
    reg_t _ric_out = _ric_scope_clash_workaround;       \
    _ric_out = FOLD_BITS_TO_1(_ric_out);                \
    _ric_out = BW_XOR(_ric_out, 1);                     \
    _ric_out;                                           \
})    

// Returns 1 if X is not equal to zero
#define BW_NEQUAL0(X) ({                                \
    const reg_t _ric_scope_clash_workaround = X;        \
    reg_t _ric_out = _ric_scope_clash_workaround;       \
    _ric_out = FOLD_BITS_TO_1(_ric_out);                \
    _ric_out;                                           \
})

// Returns 1 if X and Y are equal
#define BW_EQUAL(X, Y) ({                               \
    const reg_t _ric_scope_clash_workaround1 = X;       \
    const reg_t _ric_scope_clash_workaround2 = Y;       \
    reg_t _ric_out = BW_XOR(                            \
        _ric_scope_clash_workaround1,                   \
        _ric_scope_clash_workaround2);                  \
    _ric_out = FOLD_BITS_TO_1(_ric_out);                \
    _ric_out = BW_XOR(_ric_out, 1);                     \
    _ric_out;                                           \
})

// Returns 1 if X and Y are not equal
#define BW_NEQUAL(X, Y) ({                              \
    const reg_t _ric_scope_clash_workaround1 = X;       \
    const reg_t _ric_scope_clash_workaround2 = Y;       \
    reg_t _ric_out = BW_XOR(                            \
        _ric_scope_clash_workaround1,                   \
        _ric_scope_clash_workaround2);                  \
    _ric_out = FOLD_BITS_TO_1(_ric_out);                \
    _ric_out;                                           \
})

// Returns 1 if X > Y {OPTIMISE}
#define UINT_GTHAN(X, Y) ({                             \
    const reg_t _ric_scope_clash_workaround1 = X;       \
    const reg_t _ric_scope_clash_workaround2 = Y;       \
    reg_t _ric_out = BW_XOR(                            \
        _ric_scope_clash_workaround1,                   \
        _ric_scope_clash_workaround2);                  \
    _ric_out = GENERATE_MASK_UP_TO_MSB(_ric_out);       \
    _ric_out = UINT_SUB(_ric_out, BW_BSR(_ric_out, 1)); \
    _ric_out = BW_XOR(BW_AND(Y, _ric_out), _ric_out);   \
    _ric_out = FOLD_BITS_TO_1(_ric_out);                \
    _ric_out;                                           \
})

// Returns 1 if X >= Y {OPTIMISE}
#define UINT_GEQUAL(X, Y) ({                            \
    BW_OR(UINT_GTHAN(X,Y), BW_EQUAL(X,Y));              \
})

// Returns 1 if X < Y {OPTIMISE}
#define UINT_LTHAN(X, Y) ({                             \
    BW_XOR(UINT_GEQUAL(X, Y), 1);                       \
})

// Returns 1 if X <= Y {OPTIMISE} 
#define UINT_LEQUAL(X, Y) ({                            \
    BW_XOR(UINT_GTHAN(X, Y), 1);                        \
})

// Returns 1 if X > Y {OPTIMISE}
#define INT_GTHAN(X, Y) ({                              \
    reg_t _ric_out = UINT_GTHAN(X, Y);                  \
    reg_t _ric_signX = BW_AND(BW_BSR(                   \
        X, BOOST_PP_SUB(REGISTER_SIZE_BITS, 1)), 1);    \
    reg_t _ric_signY = BW_AND(BW_BSR(                   \
        Y, BOOST_PP_SUB(REGISTER_SIZE_BITS, 1)), 1);    \
    reg_t _ric_signYnotX = BW_AND(                      \
        _ric_signY, BW_XOR(_ric_signX, 1));             \
    reg_t _ric_signXnotY = BW_AND(                      \
        _ric_signX, BW_XOR(_ric_signY, 1));             \
    _ric_out = BW_OR(_ric_out, _ric_signYnotX);         \
    _ric_out = BW_NOT(                                  \
        BW_OR(BW_NOT(_ric_out), _ric_signXnotY));       \
    _ric_out;                                           \
})

// Returns 1 if X >= Y {OPTIMISE}
#define INT_GEQUAL(X, Y) ({                             \
    BW_OR(INT_GTHAN(X, Y), BW_EQUAL(X, Y));             \
})

// Returns 1 if X < Y {OPTIMISE}
#define INT_LTHAN(X, Y) ({                              \
    BW_XOR(INT_GEQUAL(X, Y), 1);                        \
})

// Returns 1 if X <= Y {OPTIMISE}
#define INT_LEQUAL(X, Y) ({                             \
    BW_XOR(INT_GTHAN(X, Y), 1);                         \
})


// END HEADER GUARD ... 
#endif
