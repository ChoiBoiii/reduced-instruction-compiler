
// The type to use as a 'register'
typedef unsigned char reg_t;

reg_t ADD(X, Y) {
    reg_t keep = (X & Y) << 1;
    reg_t res = X ^ Y;
    return (keep) ? ADD(keep, res) : res;
}

