#!/usr/bin/bash
#!/bin/bash

# compress for delivery
./clean.sh
zip -r ../../SO_069_proj2.zip ../bin/doTest.sh ../CircuitRouter-ParSolver ../CircuitRouter-SeqSolver ../results ../README.txt ../Makefile ../lib
