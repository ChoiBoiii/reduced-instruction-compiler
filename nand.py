def NAND(a, b):
    return ~(a & b)

def AND(a, b):
    return NAND(NAND(a, b), NAND(a, b))

def OR(a, b):
    return NAND(NAND(a, a), NAND(b, b))

def NOT(a):
    return NAND(a, a)
