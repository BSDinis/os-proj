#!/usr/bin/zsh
#!/usr/bin/bash

if [ $# -ne 1 ] 
then
  echo "usage: $0 <n_threads>";
  exit 1
fi

for file in ../inputs/*.txt
do
  echo "============================================================"
  echo " == SOLVING FILE " $file
  echo "============================================================"
  echo
  ../bin/doTest.sh $1 ../inputs/$file
done

