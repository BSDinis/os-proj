#!/usr/bin/bash
#!/bin/bash

# analyse the circuits in the input folder
# for each input file, return the number of lines in the file 
# and the number of interconnections

input_folder='../inputs'

for input in ${input_folder}/*.txt
do
  n_lines=$(wc -l ${input})
  n_circuits=$(grep '^p' ${input} | wc -l)
  echo == file ${input} ==
  echo ${n_lines} lines 
  echo ${n_circuits} connections
  echo 
done
