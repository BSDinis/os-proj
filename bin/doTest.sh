#!/usr/bin/zsh
#!/usr/bin/bash

#sanity checks
seq="../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver"
par="../CircuitRouter-ParSolver/CircuitRouter-ParSolver"

mem="no"
prof="no"
if [[ -v MEM ]]
then
  mem="yes"
  echo "Memory Checking enabled"
  echo
  echo
  echo "Starting"
elif [[ -v PROF ]]
then
  prof="yes"
  echo "Profiling enabled"
  echo
  echo
  make -C .. clean
  make -C .. PROF=yes
  echo
  echo
  echo "Starting"
elif [[ -v STORE ]]
then
  store_speedup="yes"
  echo "Storing speedup at ../results"
  echo
  echo
  make -C .. clean
  make -C .. PROF=yes
  echo
  echo
  echo "Starting"
fi


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
elif [[ $1 -lt 1 ]]
then
  echo "number of threads must be positive"
  exit 1
fi

add_new_entry () {
  if [ $# -ne 3 ]
  then
    echo "usage: $0 <file> <row_name> <time>"
    exit 1
  fi
  echo "$1"
  speedup=$(echo "scale=6; ${sequential}/$3" | bc)
  echo $2",\t\t"$3",\t"${speedup} >> $1
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


echo "#n_threads,\ttime,\t\tspeedup" >> ${speedup_file}
echo "== Running sequential =="
if [[ $mem == "yes" ]] 
then
  valgrind ../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
else
  ../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $2
fi

if [[ $prof == "yes"  ]]
then
  if [[ -a ../profiles/$2.prof ]]
  then
    mv ../profiles/$(basename $2).prof ../profiles/$(basename $2).prof.old
  fi
  echo "profiling to "../profiles/$2.prof
  gprof -p -q -r ../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver gmon.out > ../profiles/$2.prof
fi
sequential=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
add_new_entry $speedup_file 1S $sequential
echo "== Time: "$sequential
echo "== Done == "

for i in {1..$1} 
do
  echo "== Running parallel w/ "$i" threads =="
  if [[ $mem == "yes" ]]
  then
    valgrind --leak-check=full ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  else
    ../CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $2 ;
  fi
  ret=$?
  if [[ $prof == "yes" ]]
  then
    if [[ -a ../profiles/$(basename $2.$i.prof) ]]
    then
      mv ../profiles/$(basename $2.$i.prof) ../profiles/$(basename $2.$i.prof.old)
    fi
    echo "profiling to "../profiles/$(basename $2.$i.prof)
    gprof -p -q -r ../CircuitRouter-ParSolver/CircuitRouter-ParSolver gmon.out > ../profiles/$(basename $2.$i.prof)
  fi
  if [[ $ret -ne 0 ]]
  then
    echo "An error occurred: ParSolver returned "$ret". Aborting"
    exit $ret
  fi
  timed=$(grep "Elapsed time" $2.res | cut -d " " -f 5)
  add_new_entry $speedup_file $i $timed
  echo "== Time: "$timed
  echo "== Done == "
done

# wrappup
# This should be a move, but my hands were tied
if [[ $store_speedup == "yes" ]]
then
  cp $speedup_file $new_speedup_file
fi

if [[ $prof == "yes" ]]
then
  rm -f gmon.out
fi
