Project Structure

.
├── bin
│   ├── analyse-input.sh 
│   └── clean-res.sh # removes all *.res and *.res.old in inputs
├── CircuitRouter-ParSolver
│   ├── CircuitRouter-ParSolver.c
│   ├── coordinate.c
│   ├── coordinate.h
│   ├── grid.c
│   ├── grid.h
│   ├── Makefile
│   ├── maze.c
│   ├── maze.h
│   ├── router.c
│   ├── router.h
│   └── test_args.py
├── CircuitRouter-SeqSolver
│   ├── autodep
│   ├── CircuitRouter-SeqSolver
│   ├── CircuitRouter-SeqSolver.c
│   ├── CircuitRouter-SeqSolver.o
│   ├── coordinate.c
│   ├── coordinate.h
│   ├── coordinate.o
│   ├── grid.c
│   ├── grid.h
│   ├── grid.o
│   ├── Makefile
│   ├── maze.c
│   ├── maze.h
│   ├── maze.o
│   ├── router.c
│   ├── router.h
│   ├── router.o
│   └── test_args.py
├── CircuitRouter-SimpleShell
│   ├── CircuitRouter-SimpleShell.c
│   ├── command.c
│   ├── command.h
│   ├── gen_inputs.py
│   └── Makefile
├── inputs
│   ├── generate.py
│   ├── random-x128-y128-z3-n128.txt
│   ├── random-x128-y128-z3-n64.txt
│   ├── random-x128-y128-z5-n128.txt
│   ├── random-x256-y256-z3-n256.txt
│   ├── random-x256-y256-z5-n256.txt
│   ├── random-x32-y32-z3-n64.txt
│   ├── random-x32-y32-z3-n64.txt.res
│   ├── random-x32-y32-z3-n64.txt.res.old
│   ├── random-x32-y32-z3-n96.txt
│   ├── random-x32-y32-z3-n96.txt.res
│   ├── random-x32-y32-z3-n96.txt.res.old
│   ├── random-x48-y48-z3-n48.txt
│   ├── random-x48-y48-z3-n48.txt.res
│   ├── random-x48-y48-z3-n48.txt.res.old
│   ├── random-x48-y48-z3-n64.txt
│   ├── random-x48-y48-z3-n64.txt.res
│   ├── random-x48-y48-z3-n64.txt.res.old
│   ├── random-x512-y512-z7-n512.txt
│   ├── random-x64-y64-z3-n48.txt
│   ├── random-x64-y64-z3-n48.txt.res
│   ├── random-x64-y64-z3-n48.txt.res.old
│   ├── random-x64-y64-z3-n64.txt
│   ├── random-x64-y64-z3-n64.txt.res
│   └── random-x64-y64-z3-n64.txt.res.old
├── lib
│   ├── commandlinereader.c
│   ├── commandlinereader.h
│   ├── hashtable.c
│   ├── hashtable.h
│   ├── list.c
│   ├── list.h
│   ├── Makefile
│   ├── pair.c
│   ├── pair.h
│   ├── queue.c
│   ├── queue.h
│   ├── simple_list.c
│   ├── simple_list.h
│   ├── timer.h
│   ├── types.h
│   ├── utility.h
│   ├── vector.c
│   └── vector.h
├── README.txt
└── test
    ├── Makefile
    ├── test_hashtable.c
    └── test_simple_list.c

7 directories, 83 files
