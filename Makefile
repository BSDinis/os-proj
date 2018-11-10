all: par seq #ssh 

#ssh: CircuitRouter-SimpleShell
	#make -C CircuitRouter-SimpleShell $@

par: 
	make -C CircuitRouter-ParSolver 

seq: 
	make -C CircuitRouter-SeqSolver 

clean: 
	#make -C CircuitRouter-SimpleShell $@
	make -C CircuitRouter-ParSolver $@
	make -C CircuitRouter-SeqSolver $@
