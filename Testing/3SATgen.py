'''

    This Script will generate a random set of sat problems and put them into multiple txt files
    formatted. the files need to follow this format:

        c This file contains the Einstain's puzzle expressed in CNF
        p cnf 5 5
        1 2 -5 0
        -3 -4 5 0
        -1 3 4 0
        -1 -2 -4 0
        4 -4 0

'''
import os
import random
problem_dir = "./problems"
if not os.path.isdir(problem_dir):
    os.makedirs(problem_dir)

count = int(input("Number of problems to make?"))
N = int(input("Number of variables?"))
L = int(input("Number of clauses?"))
K = 3
def fixed_clause_length(n,k):
    clause = ""
    for i in range(k):
        literal = random.randrange(n)
        sign = -1 if random.random() < 0.5 else 1
        literal_str = str(literal*sign)
        clause += "{} ".format(literal_str)
    clause+="0\n"
    return clause
def start(n,l,k):
    for i in range(count):
        with open("{}/problem{}.cnf".format(problem_dir,i),"w+") as f:
            f.write("c This file contains test problem\n")
            f.write("p cnf {} {}\n".format(n,l))
            for j in range(l):
                f.write(fixed_clause_length(n,k))

start(N,L,K)
