#!/usr/bin/zsh
#!/usr/bin/bash

for file in ../inputs/*.txt
do
  echo "============================================================"
  echo " == SOLVING FILE " $file
  echo "============================================================"
  echo
  ./doTest.sh $1 ../inputs/$file
done

