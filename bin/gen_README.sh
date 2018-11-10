#!/usr/bin/zsh

out=../README.txt
if [[ -a $out ]]
then
  rm -f $out;
fi


function print_everything() {
  echo "SO Projeto 2 - Grupo 69 - Baltasar Dinis" 
  echo 
  echo README generated at: $(date)
  echo
  echo "STRUCTURE"
  echo "  (relative to project root, where README.txt is)"
  echo 
  echo "Makefile: global makefile"
  echo 
  echo "CircuitRouter-ParSolver: solution of the project"
  echo "  Contains: Makefile, src files"
  echo 
  echo "CircuitRouter-SeqSolver: sequential router solver"
  echo "  Contains: Makefile, src files"
  echo 
  echo "results: speedup data from tests"
  echo "  Contains: *.speedups.csv files"
  echo 
  echo "lib: library of utilities (provided)"
  echo "  Contains: Makefile, src files"
  echo 
  echo "bin: script folder"
  echo "  Contains: doTest.sh  "
  echo 
  echo
  echo "HOW TO COMPILE"
  echo "Easy: make"
  echo 
  echo "HOW TO RUN (after compiling)"
  echo 
  echo "Via doTest.sh: run ./bin/doTest.sh <max number of threads> <input file>"
  echo "Via binary:    run ./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t <number of threads> <input file>"
  echo 
  echo "OS information:"
  echo $(uname -a)
  echo
  echo "CPU information:"
  echo "Via cat /proc/cpuinfo:"
  echo $(cat /proc/cpuinfo)
  echo
  echo "Via lscpu:"
  echo $(lscpu)
}



print_everything > $out
print_everything 
