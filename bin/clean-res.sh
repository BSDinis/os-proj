#!/usr/bin/bash
#!/bin/bash

# analyse the circuits in the input folder
# for each input file, return the number of lines in the file 
# and the number of interconnections

input_folder='../CircuitRouter-SeqSolver/inputs'

for input in ${input_folder}/*.txt.res*
do
  echo == removing file ${input} ==
  rm ${input}
done
