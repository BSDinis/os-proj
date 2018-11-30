#!/usr/bin/zsh
#!/usr/bin/bash

if [ $# -lt 1 ]
then
  echo "dude... how many threads?"
  exit 1
fi


# got tired of thinking which file to run the project on
file=../inputs/random-x48-y48-z3-n64.txt

echo "============================================================"
echo " == SOLVING FILE " $file
echo "============================================================"
echo
../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $1 $file ./abc.pipe

echo "============================================================"
echo " == SOLVED " $file
echo "============================================================"
