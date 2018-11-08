#!/usr/bin/zsh

echo "==          DO TEST         =="
echo "==  run tests on ParSolver  =="
echo "== max number of threads: "$1" =="
echo "== inpufile: "$2" =="

echo "== Running sequential =="
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
echo "== Done == "

for i in {1..$1} 
do
  echo "== Running parallel w/ "$i" threads =="
  ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  grep routed ${2}.res.old
  grep routed ${2}.res
  echo "== Done == "
done

