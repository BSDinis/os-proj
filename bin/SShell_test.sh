#!/usr/bin/zsh

if ! [[ -x ../CircuitRouter-SimpleShell/CircuitRouter-SimpleShell ]]
then
  echo "Found no executable"
  echo "abort"
  exit 1
fi


./gen_shell_inputs.py | ../CircuitRouter-SimpleShell/CircuitRouter-SimpleShell
