#!/usr/bin/bash
#!/bin/bash

# analyse the circuits in the input folder
# for each input file, return the number of lines in the file 
# and the number of interconnections

input_folder='../inputs'

for input in ${input_folder}/*.txt.*
do
  # test for empty case
  if [ "$input" != "../inputs/*.txt.*" ]; then
    echo == removing file ${input} ==
    rm ${input}
  fi
done

rm -f vgcore*

make -C ../CircuitRouter-ParSolver clean
make -C ../CircuitRouter-SeqSolver clean
make -C ../lib clean
