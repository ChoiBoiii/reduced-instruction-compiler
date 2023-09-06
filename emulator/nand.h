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
    | NEQUAL        | Bitwise inequality              | f(a, b) -> (a != b)   | 0 or 1   |                         |
    | NEQUAL0       | Bitwise inequality with zero    | f(a)    -> (a != 0)   | 0 or 1   | T                       |
    | EQUAL         | Bitwise equality                | f(a, b) -> (a == b)   | 0 or 1   |                         |
    | EQUAL0        | Bitwise equality with zero      | f(a)    -> (a == 0)   | 0 or 1   | T                       |
    | UINT_ADD      | Add two unsigned integers       | f(a, b) -> (a + b)    | 
    | UINT_SUB      | Subtract two unsigned integers  | f(a, b) -> (a - b)    |
    | INT_ADD       | Signed integer addition         | f(a, b) -> (a + b)    |
    | UINT_MULT     | Unsigned integer multiplication | f(a, b) -> (a * b)    |
    | INT_MULT      | Signed integer multiplication   | f(a, b) -> (a * b)    | 
    | INT_DIV       | Signed integer division         | f(a, b) -> int(a / b) | 
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
#define EQUAL0(X) ({      \
    reg_t v = X;           \
    v = OR(v, BSR(v, 64)); \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v;                     \
})    

// Returns 1 if X is not equal to zero
#define NEQUAL0(X) ({      \
    reg_t v = X;           \
    v = OR(v, BSR(v, 64)); \
    v = OR(v, BSR(v, 32)); \
    v = OR(v, BSR(v, 16)); \
    v = OR(v, BSR(v, 8));  \
    v = OR(v, BSR(v, 4));  \
    v = OR(v, BSR(v, 2));  \
    v = OR(v, BSR(v, 1));  \
    v = AND(v, 1);         \
    v;                     \
})    


// TODO ...






















#define NEQUAL(X, Y)
#define EQUAL(X, Y)   ( NOT(XOR(X, Y))               ) // NEEDS FIX - True if both numbers bitpatterns are the same


// MATHEMATICAL OPERATORS ...
/* Adds two given unsigned integers
reg_t ADD_UNSIGNED(X, Y) {
    reg_t tmp;
    reg_t keep = BSL(AND(X, Y), 1);
    reg_t res = XOR(X, Y); 
    loop:
        tmp = keep;
        keep = BSL(AND(keep, res), 1);
        res = XOR(tmp, res);
        if (keep)
            goto loop;
    return res;
}
*/
#define ADD_UNSIGNED(X, Y) ({reg_t t,k,r;k=BSL(AND(X,Y),1);r=XOR(X,Y);l:t=k;k=BSL(AND(k,r),1);r=XOR(t,r);if(k)goto l;r;})

// Subtracts b from a
#define SUB_UNSOGNED(X, Y) ({})

// Adds two signed integers
#define ADD_SIGNED(X, Y) ({})

// Multiplies given unsigned integers
#define MULT_UNSIGNED(X, Y) ({})

// Performs integer division of X/Y
#define INT_DIV(X, Y) ({})



