#!/usr/bin/python3
import os
import random
from datetime import datetime

random.seed(datetime.now())

BASE_DIR = "../CircuitRouter-SeqSolver/"
BASENAME_CMD = "CircuitRouter-SeqSolver"

CMD = BASE_DIR + BASENAME_CMD

def genRand():
  legalOptions = ['b', 'x', 'y', 'z', 'h']
  filename = [
    'random-x32-y32-z3-n64.txt' ,
    'random-x32-y32-z3-n96.txt' ,
    'random-x48-y48-z3-n48.txt' ,
    'random-x48-y48-z3-n64.txt' ,
    'random-x64-y64-z3-n48.txt' ,
    'random-x64-y64-z3-n64.txt' 
  ]

  legal = random.randint(0, 5)
  _file = random.randint(0, 1)
  if _file != 0:
    _file = 1

  res = str()
  while legal > 0 or _file > 0:
    typ = random.randint(0, 1)
    if typ == 0 and legal > 0:
      optionN = random.randint(0, len(legalOptions) - 1)
      option = legalOptions[optionN];
      res += ' -' + option
      if option != 'h':
        res += ' ' + str(random.randint(0, 10))
      del(legalOptions[optionN]);
      legal -= 1

    elif typ == 1 and _file > 0:
      filen = filename[random.randint(0, len(filename) -1)]
      res += ' inputs/'+filen
      _file -= 1

  return res

def run():
  for i in range(10):
    cmd = CMD+genRand()
    print('\n' + cmd + '\n\n\n\n')
  os.system(cmd)

cmd = 'valgrind '+CMD+genRand()
print(cmd)
os.system(cmd)
