#!/usr/bin/zsh

if ! [[ -x ../CircuitRouter-AdvShell/CircuitRouter-AdvShell ]]
then
  echo "Found no executable"
  echo "abort"
  exit 1
fi

./gen_shell_inputs.py > .a.tmp
../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp

valgrind ../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp
echo "" > .a.tmp
valgrind ../CircuitRouter-AdvShell/CircuitRouter-AdvShell < .a.tmp

rm .a.tmp

