### Makefile for OS project

CC := gcc

INCLUDES := -I. -Ilib

# vpath: tells where to search for files
VPATH := .

# fdiagnostics... make the output colorized
CFLAGS := -Wall -Werror -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -L -Wall -Werror -fdiagnostics-color=always $(INCLUDES) 
LDLIBS := -lpthread -lm # link pthreads and math functs
LDFLAGS += -L./lib # search the lib dir for libraries
LDLIBS += -lutils # link the utils library

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
all: depend utils $(TARGETS)

-include autodep

# create static lib
# -C means it goes into the lib dir and runs make
utils:
	@make -C lib

# create executable
CircuitRouter-SeqSolver: $(OBJECTS) 

# PHONY means it always runs 
# (doesn't check if the dependencies didn't change)
#
# again, goes into the lib dir and cleans
# the -f flag supresses outpu if there are no files
.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGETS)
	@make clean -C lib

# get dependencies
.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
