
// The type to use as a 'register'
typedef unsigned char reg_t;

// Decomposed bitwise instructions
#define NAND(X, Y) ( ~(X & Y) )
#define AND(X, Y)  ( NAND(NAND(X, Y), NAND(X, Y)) )
#define OR(X, Y)   ( NAND(NAND(X, X), NAND(Y, Y)) )
#define NOT(X)     ( NAND(X, X)                   )
#define XOR(X, Y)  ( AND(OR(X, Y), NAND(X, Y))    )

// Adds two given unsigned integers
reg_t ADD_UNSIGNED(reg_t X, reg_t Y) {
    return ((AND(X, Y) << 1) != 0) ? ADD_UNSIGNED((AND(X, Y) << 1), XOR(X, Y)) : XOR(X, Y);
}

