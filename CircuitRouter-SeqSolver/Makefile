### Makefile for OS project

CC := gcc

INCLUDES := -I.. 

# vpath: tells where to search for files
VPATH := .:..

# fdiagnostics... make the output colorized
CFLAGS := -Wall -Werror -Wno-error=stringop-overflow= -std=gnu99 -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -L -fdiagnostics-color=always $(INCLUDES) 
LDLIBS := -lm # link math functs
LDFLAGS += -L.. -L../lib # search the lib dir for libraries
LDLIBS += -lutils # link the utils library

LIBUTILS = ../lib/libutils.a

# if you run 'make DEBUG=no' it will compile without the debugger flag
ifneq ($(strip $(DEBUG)), no)
 CFLAGS += -g
endif

# if you run 'make OPTIM=no' it will compile without the optimizations
ifneq ($(strip $(OPTIM)), no)
 CFLAGS += -O2
endif

# SOURCES is a list of all the files in the current dir with a .c extension
# OBJECTS is a list created by taking SOURCES and replacing the .c extension with .o
# TARGETS is the target executable
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = CircuitRouter-SeqSolver

# depend creates autodep, which parses the files and
# creates rules based on their dependencies
#
# utils is a target that recompiles the library
all: depend $(LIBUTILS) $(TARGETS)

-include autodep

# create static lib
# -C means it goes into the lib dir and runs make
$(LIBUTILS):
	@make -C ../lib

# create executable
CircuitRouter-SeqSolver: $(OBJECTS) 

# PHONY means it always runs 
# (doesn't check if the dependencies didn't change)
#
# again, goes into the lib dir and cleans
# the -f flag supresses outpu if there are no files
.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGETS) autodep vgcore*
	@make clean -C ../lib

# get dependencies
.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
