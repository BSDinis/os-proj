all: par seq sish ash

ash: 
	make -C CircuitRouter-AdvShell 


sish: 
	make -C CircuitRouter-SimpleShell 

par: 
	make -C CircuitRouter-ParSolver 

seq: 
	make -C CircuitRouter-SeqSolver 

clean: 
	make -C CircuitRouter-AdvShell $@
	make -C CircuitRouter-SimpleShell $@
	make -C CircuitRouter-ParSolver $@
	make -C CircuitRouter-SeqSolver $@
