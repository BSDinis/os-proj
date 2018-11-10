#!/usr/bin/zsh
#!/usr/bin/bash

#sanity checks
seq="../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver"
par="../CircuitRouter-ParSolver/CircuitRouter-ParSolver"

if [ $# -ne 2 ]
then
  echo "usage: $0 <n_threads> <input_file>"
  exit 1
elif [[ ! -r $2 ]]
then
  echo "file $2 not found"
  exit 1
elif [[ ! -x $seq || ! -x $par ]]
then
  echo "executables not found"
  exit 1
fi

add_new_entry () {
  #if [ $# -ne 3 ]
  #then
    #echo "usage: $0 <file> <row_name> <time>"
    #exit 1
  #fi
  echo "$1"
  speedup=$(echo "scale=6; ${sequential}/$3" | bc)
  echo $2",\t\t\t\t"$3",\t"${speedup} >> $1
}


#output files
speedup_file=$2.speedups.csv 
new_speedup_file=../results/$(basename ${speedup_file})
echo -n "" > ${speedup_file}
echo -n "" > ${new_speedup_file}


echo "==          DO TEST         =="
echo "==  run tests on ParSolver  =="
echo "== max number of threads: "$1" =="
echo "== inpufile: "$2" =="
echo "== speedup_file: "${new_speedup_file}" =="


echo "#n_threads,\t\ttime,\t\tspeedup" >> ${speedup_file}
echo "== Running sequential =="
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
sequential=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
add_new_entry $speedup_file 1S $sequential
echo "== Time: "$sequential
echo "== Done == "

for i in {1..$1} 
do
  echo "== Running parallel w/ "$i" threads =="
  ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  timed=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
  add_new_entry $speedup_file $i $timed
  echo "== Time: "$timed
  echo "== Done == "
done

# wrappup
# This should be a move, but my hands were tied
cp $speedup_file $new_speedup_file
