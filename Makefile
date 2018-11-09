all: par seq

par: 
	make -C CircuitRouter-ParSolver

seq: 
	make -C CircuitRouter-SeqSolver
