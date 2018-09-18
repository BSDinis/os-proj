### Makefile for OS project

CC := gcc

INCLUDES := -I. -Ilib
VPATH := .

# missing -Werror
CFLAGS := -Wall -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -L -Wall -Werror -fdiagnostics-color=always $(INCLUDES) 
LDLIBS := -lpthread -lm
LDFLAGS += -L./lib
LDLIBS += -lutils

ifneq ($(strip $(DEBUG)), no)
  CFLAGS += -g
endif

ifneq ($(strip $(OPTIM)), yes)
  CFLAGS += -O2
endif

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = CircuitRouter-SeqSolver

all: depend utils $(TARGETS)

-include autodep

# create static lib
utils:
	@make -C lib

# create executable
CircuitRouter-SeqSolver: $(OBJECTS) 

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGETS)
	@make clean -C lib

# get dependencies
.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
