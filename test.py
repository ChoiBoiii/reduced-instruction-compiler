a = "     # include    <path>     \nnext line\n"

a = a.split('\n')


for line in a:
    line = line.split()
    print(line)