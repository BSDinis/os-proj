#!/usr/bin/zsh

if ! [[ -x ../CircuitRouter-AdvShell/CircuitRouter-AdvShell ]]
then
  echo "Found no executable"
  echo "abort"
  exit 1
fi

./gen_shell_inputs.py $1 $2 > .a.tmp

echo "====================================="
echo "=========== Regular Tests ==========="
echo "====================================="
../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp

echo "====================================="
echo "=========== Valgrind Tests =========="
echo "====================================="
valgrind ../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp
echo "" > .a.tmp

echo "====================================="
echo "============= Extra Tests ==========="
echo "====================================="
valgrind ../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp

rm .a.tmp

