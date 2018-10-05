/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *
  * Simple Shell to spawn CircuitSolver-SeqSolver processes
  *
  * Interface
  * 
  * run filename (solve the Circuit at that filename)
  * exit (exit the application, printing all the results from the processes)
  * =============================================================================
  *
  * CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */


#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int64_t global_max_children;

/* =============================================================================
  * displayUsage
  * =============================================================================
  */
static void display_usage(const char* program_name){
  printf("Usage: %s <max_children (>0) > (default: unlimited)\n", program_name);
  exit(1);
}

/* =============================================================================
  * setDefaults
  * =============================================================================
  */
static void setDefaults ()
{
  global_max_children = -1;
}


/* =============================================================================
  * parseArgs
  * =============================================================================
  */
static void parseArgs (long argc, char* const argv[]){
  if (argc < 1 || argc > 2) {
    display_usage(argv[0]);
  }
  else if (argc == 2) {
    int _errno = errno; // push errno
    errno = 0;
    global_max_children = strtoll(argv[1], NULL, 10);
    if (errno != 0) {
      perror("parseArgs: strtoll");
      display_usage(argv[0]);
    }
    else if (global_max_children <= 0) {
      display_usage(argv[0]);
    }
    errno = _errno; // pop errno
  }
  else {
    setDefaults();
  }
}

/* =============================================================================
  * main
  * =============================================================================
  */
int main(int argc, char** argv){
  /*
  * Initialization
  */
  parseArgs(argc, (char** const)argv);
  printf("hi\nmaxchildren = %ld\n", global_max_children);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
