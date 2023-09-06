
// The type to use as a 'register'
typedef unsigned char reg_t;

reg_t ADD(X, Y) {
    reg_t keep = (X & Y) << 1;
    reg_t res = X ^ Y;
    return (keep) ? ADD(keep, res) : res;
}



reg_t ADD(X, Y) {
    reg_t keep = (X & Y) << 1;
    reg_t res = X ^ Y;
    loopback:
        keep = (AND(keep, res)) << 1;
        res = XOR(keep, res);
        if (keep)
            return res;
    goto loopback;

    // return (keep) ? ADD(keep, res) : res;
}

