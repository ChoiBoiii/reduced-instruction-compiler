
// The type to use as a 'register'
typedef u_int32_t reg_t;

// Base operators
#define NAND(X, Y) ( ~(X & Y) )                     // NAND bitwise operator
#define BSL(X, N)  ( (X << N)                     ) // Bitshift left operator     

// Decomposed bitwise instructions
#define AND(X, Y)  ( NAND(NAND(X, Y), NAND(X, Y)) ) // AND bitwise operator
#define OR(X, Y)   ( NAND(NAND(X, X), NAND(Y, Y)) ) // OR bitwise operator
#define NOT(X)     ( NAND(X, X)                   ) // NOT bitwise operator
#define XOR(X, Y)  ( AND(OR(X, Y), NAND(X, Y))    ) // XOR bitwise operator

// Adds two given unsigned integers
reg_t ADD_UNSIGNED(reg_t X, reg_t Y) {
    return ((AND(X, Y) << 1) != 0) ? ADD_UNSIGNED((AND(X, Y) << 1), XOR(X, Y)) : XOR(X, Y);
}



// reg_t ADD(X, Y) {
//     reg_t keep = (X & Y) << 1;
//     reg_t res = X ^ Y;
//     return (keep) ? ADD(keep, res) : res;
// }


reg_t ADD(X, Y) {
    reg_t tmp;
    reg_t keep = BSL(AND(X, Y), 1);
    reg_t res = XOR(X, Y); 
    loopback:
        tmp = keep;
        keep = BSL(AND(keep, res), 1);
        res = XOR(tmp, res);
        if (keep)
            goto loopback;
    return res;
}
