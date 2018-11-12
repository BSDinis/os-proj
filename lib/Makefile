### Makefile for OS project

CC := gcc
INCLUDES := -I.
VPATH := .

CFLAGS := -Wall -Werror -fdiagnostics-color=always $(INCLUDES)
LDFLAGS := -L -fdiagnostics-color=always $(INCLUDES)

# if you run 'make PROF=yes' it will compile with information for profiler
ifeq ($(strip $(PROF)), yes)
  CFLAGS += -pg
  LDFLAGS += -pg
endif

ifneq ($(strip $(DEBUG)), no)
  CFLAGS += -g
endif

ifneq ($(strip $(OPTIM)), yes)
  CFLAGS += -O2
endif


SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGETS = libutils.a

all: depend $(TARGETS)

-include autodep

libutils.a: $(OBJECTS)
	 ar rcs $@ $^

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGETS) autodep

.PHONY: depend
depend: $(SOURCES)
	$(CC) $(INCLUDES) -MM $(SOURCES) > autodep
