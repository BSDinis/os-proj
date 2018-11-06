#!/usr/bin/zsh

echo "==          DO TEST         =="
echo "==  run tests on ParSolver  =="
echo "== max number of threads: "$1" =="

echo "== Running sequential =="
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
echo "== Done == "

for i in {1..$1} 
do
  echo "== Running parallel w/ "$i" threads =="
  ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  echo "== Done == "
done

