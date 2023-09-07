/* 

    DESCRIPTION
    A header only library allowing code that can be compiled to all NAND's. Why? why not :)

    INSTRUCTIONS
    |--------------------------------------------------------------------------------------------------------------|
    | Instruction   | Description                     | Equivalent Operator   | Returns  | Register Size Dependant |
    |--------------------------------------------------------------------------------------------------------------|
    | NAND          | NAND Bitwise operator           | f(a, b) -> ~(a & b)   | Bitfield | F                       |
    | BSL           | Bitshift left operator          | f(a, n) -> (a << n)   | Bitfield | F                       |
    | BSR           | Bitshift right operator         | f(a, n) -> (a >> n)   | Bitfield | F                       |
    | AND           | AND bitwise operator            | f(a, b) -> (a & b)    | Bitfield | F                       |
    | OR            | OR bitwise operator             | f(a, b) -> (a | b)    | Bitfield | F                       |
    | NOT           | NOT bitwise operator            | f(a)    -> (~a)       | Bitfield | F                       |
    | XOR           | XOR bitwise operator            | f(a, b) -> (a ^ b)    | Bitfield | F                       |
    | NEQUAL        | Bitwise inequality              | f(a, b) -> (a != b)   | 0 or 1   | T                       |
    | NEQUAL0       | Bitwise inequality with zero    | f(a)    -> (a != 0)   | 0 or 1   | T                       |
    | EQUAL         | Bitwise equality                | f(a, b) -> (a == b)   | 0 or 1   | T                       |
    | EQUAL0        | Bitwise equality with zero      | f(a)    -> (a == 0)   | 0 or 1   | T                       |
    | GREATER_THAN  | Bitwise greater than            | f(a, b) -> (a > b)    | 0 or 1   | 
    | GEQUAL        | Bitwise greater or equal to     | f(a, b) -> (a >= b)   | 0 or 1   |
    | LESS_THAN     | Bitwise less than               | f(a, b) -> (a < b)    | 0 or 1   |
    | LEQUAL        | Bitwise less than or equal to   | f(a, b) -> (a <= b)   | 0 or 1   | 
    | UINT_ADD      | Add two unsigned integers       | f(a, b) -> (a + b)    | n        | T                       |
    | UINT_SUB      | Subtract two unsigned integers  | f(a, b) -> (a - b)    | n        |                         |
    | UINT_MULT     | Unsigned integer multiplication | f(a, b) -> (a * b)    | n        |                         |
    | UINT_DIV      | Unsigned integer division       | f(a, b) -> int(a / b) | n        |                         |
    | INT_ADD       | Signed integer addition         | f(a, b) -> (a + b)    | n        | T                       |
    | INT_MULT      | Signed integer multiplication   | f(a, b) -> (a * b)    | n        |                         |
    | INT_DIV       | Signed integer division         | f(a, b) -> int(a / b) | n        |                         |
    |---------------------------------------------------------------------------------------------------------------

    NOTE
    * If an instruction is tagged with 'Register Size Dependant' its implementation is dependant on register size and must be
      updated if the specified register size (currently 64-bit) is changed.


*/

// CONFIG ...

// The type to use as a 'register'
typedef u_int64_t reg_t;


// BITWISE OPERATORS ...

// Base operators (not NAND based)
#define NAND(X, Y)    ( ~(X & Y)                     ) 
#define BSL(X, N)     ( (X << N)                     ) 
#define BSR(X, N)     ( (X >> N)                     ) 

// Other decomposed bitwise operators
#define AND(X, Y)     ( NAND(NAND(X, Y), NAND(X, Y)) ) 
#define OR(X, Y)      ( NAND(NAND(X, X), NAND(Y, Y)) ) 
#define NOT(X)        ( NAND(X, X)                   ) 
#define XOR(X, Y)     ( AND(OR(X, Y), NAND(X, Y))    ) 


// EQUALITY OPERATORS ...

// Returns 1 if X is equal to zero
#define EQUAL0(X) ({       \
    reg_t v = X;           \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v = XOR(v, 1);         \
    v;                     \
})    

// Returns 1 if X is not equal to zero
#define NEQUAL0(X) ({      \
    reg_t v = X;           \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v;                     \
})

// Returns 1 if X and Y are not equal
#define NEQUAL(X, Y) ({    \
    reg_t v = XOR(X, Y);   \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v;                     \
})

// Returns 1 if X and Y are equal
#define EQUAL(X, Y) ({     \
    reg_t v = XOR(X, Y);   \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v = XOR(v, 1);         \
})

// Returns 1 if X > Y
#define GREATER_THAN(X, Y) 

// Returns 1 if X >= Y
#define GEQUAL(X, Y)

// Returns 1 if X < Y
#define LESS_THAN(X, Y)

// Returns 1 if X <= Y
#define LEQUAL(X, Y)


// MATHEMATICAL OPERATORS ...

// Inverts the sign of the given int
#define INT_SIGN_INVERT(X) ({})

// Unsigned integer addition of X+Y
#define UINT_ADD_HELPER(X, Y) ({       \
    tmp = keep;                        \
    keep = BSL(AND(keep, res), 1);     \
    res = XOR(tmp, res);               \
})
#define UINT_ADD_HELPER_4(X, Y) ({     \
    UINT_ADD_HELPER(X, Y);             \
    UINT_ADD_HELPER(X, Y);             \
    UINT_ADD_HELPER(X, Y);             \
    UINT_ADD_HELPER(X, Y);             \
})   
#define UINT_ADD_HELPER_16(X, Y) ({    \
    UINT_ADD_HELPER_4(X, Y);           \
    UINT_ADD_HELPER_4(X, Y);           \
    UINT_ADD_HELPER_4(X, Y);           \
    UINT_ADD_HELPER_4(X, Y);           \
})
#define UINT_ADD_HELPER_64(X, Y) ({    \
    UINT_ADD_HELPER_16(X, Y);          \
    UINT_ADD_HELPER_16(X, Y);          \
    UINT_ADD_HELPER_16(X, Y);          \
    UINT_ADD_HELPER_16(X, Y);          \
})
#define UINT_ADD(X, Y) ({              \
    reg_t tmp, keep, res;              \
    keep = BSL(AND(X, Y), 1);          \
    res = XOR(X, Y);                   \
    UINT_ADD_HELPER_64(X, Y);          \
    res;                               \
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
