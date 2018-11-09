#!/usr/bin/zsh
#!/usr/bin/bash

if [ $# -ne 2 ]
then
  echo "usage: ./doTest.sh <n_threads> <input_file>"
  exit 1
fi

speedup_file=$2.speedups.csv

extract_time () {
  echo $timed
  return 
}

add_new_entry () {
  speedup=$(echo "scale=6; ${sequential}/$3" | bc)
  echo $2", "$3", "${speedup} >> ${speedup_file}
}


echo "==          DO TEST         =="
echo "==  run tests on ParSolver  =="
echo "== max number of threads: "$1" =="
echo "== inpufile: "$2" =="
echo "== speedup_file: "${speedup_file}" =="


# create empty file
rm -f ${speedup_file}
touch ${speedup_file}
echo "#nofthreads, time, speedup" >> ${speedup_file}

echo "== Running sequential =="
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
sequential=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
add_new_entry $2.res 1S $sequential
echo "== Time: "$sequential
echo "== Done == "

for i in {1..$1} 
do
  echo "== Running parallel w/ "$i" threads =="
  ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  timed=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
  add_new_entry $2.res $i $timed
  echo "== Done == "
done
