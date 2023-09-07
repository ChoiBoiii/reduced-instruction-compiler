import numpy as np


## The type of the register
REG_TYPE = np.uint64 


## Base NAND instruction
def NAND(a: REG_TYPE, b: REG_TYPE) -> REG_TYPE:
    return ~(a & b)


## Base bitwise instructions
def AND(a: REG_TYPE, b: REG_TYPE) -> REG_TYPE:
    return NAND(NAND(a, b), NAND(a, b))

def OR(a: REG_TYPE, b: REG_TYPE) -> REG_TYPE:
    return NAND(NAND(a, a), NAND(b, b))

def NOT(a: REG_TYPE) -> REG_TYPE:
    return NAND(a, a)


## Base 
def BSL(a: REG_TYPE, n: REG_TYPE) -> REG_TYPE:
    if n < 0:
        print("ERROR: Invalid bitshift given")
    return a << n

def BSR(a: REG_TYPE, n: REG_TYPE) -> REG_TYPE:
    if n < 0:
        print("ERROR: Invalid bitshift given")
    return a >> n

## MATHS
def ADD(a: REG_TYPE, b: REG_TYPE) -> REG_TYPE:
    keep = (a & b) << REG_TYPE(1)
    res = a ^ b

    if keep == REG_TYPE(0):
        return res
    return ADD(keep, res)



## Test
def main():

    a = REG_TYPE(0b1010)
    b = REG_TYPE(0b1001)
    test = REG_TYPE(0b100)

    # print("NOT:", bin(NOT(test)))
    # print("AND:", bin(AND(a, b)))
    # print("OR:",  bin(OR(a, b)))
    # print("BSL:", bin(BSL(test, REG_TYPE(2)))) 
    # print("BSR:", bin(BSR(test, REG_TYPE(2)))) 

    print(ADD(REG_TYPE(456767859), REG_TYPE(123243536)))


## Run test
if __name__ == '__main__':
    main()



