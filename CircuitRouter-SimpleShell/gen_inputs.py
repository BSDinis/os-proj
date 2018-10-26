#!/usr/bin/python3
import sys
import random

heavy = [
    'random-x256-y256-z3-n256.txt',
    'random-x256-y256-z5-n256.txt',
    'random-x512-y512-z7-n512.txt'
]




input_list = [
    'random-x128-y128-z3-n128.txt',
    'random-x128-y128-z3-n64.txt',
    'random-x128-y128-z5-n128.txt',
    'random-x32-y32-z3-n64.txt',
    'random-x32-y32-z3-n96.txt',
    'random-x48-y48-z3-n48.txt',
    'random-x48-y48-z3-n64.txt',
    'random-x64-y64-z3-n48.txt',
    'random-x64-y64-z3-n64.txt'
]

for i in range(len(input_list)):
    input_list[i] = "run ../inputs/"+input_list[i];

for i in range(len(heavy)):
    heavy[i] = "run ../inputs/"+heavy[i];

for a in sys.argv:
    if a == 'heavy':
        input_list += heavy

bad_inputs = [
    'dddas asdf',
    'run ../inputs/random-x64-y64-z3-n48.txt asd',
    ' exit'
]


l = input_list

for a in sys.argv:
    if a == 'bad':
        l += bad_inputs

l = random.sample(l, len(l))

for _ in l:
    print(_)

print("exit")
