/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Adv Shell to spawn CircuitSolver-ParSolver processes
 * and receive remote requests
 *
 * Interface
 * 
 * run filename (solve the Circuit at that filename)
 * =============================================================================
 *
 * CircuitRouter-Client.c
 *
 * =============================================================================
 */


#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>

#include "lib/vector.h"

#define SHELL "../CircuitRouter-AdvShell/CircuitRouter-AdvShell"
#define SHELL_PIPE "../CircuitRouter-AdvShell/CircuitRouter-AdvShell.pipe"

char * pipe_pathname = NULL;

/* =============================================================================
  * displayUsage
  * =============================================================================
  */
static void display_usage(const char * program_name){
  printf("Usage: %s <max_children (>0) > (default: unlimited)\n", program_name);
  exit(1);
}

/* =============================================================================
  * setDefaults
  * =============================================================================
  */
static void setDefaults ()
{
}

/* =============================================================================
  * parseArgs
  * =============================================================================
  */
static void parseArgs (const long argc, char* const argv[]){
  if (argc < 1 || argc > 2) {
    display_usage(argv[0]);
  }
  else if (argc == 2) {
    errno = 0;
    /*
    global_max_children = strtoll(argv[1], NULL, 10);
    if (errno != 0) {
      perror("parseArgs: strtoll");
      display_usage(argv[0]);
    }
    else if (global_max_children <= 0) {
      display_usage(argv[0]);
    }
    */
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
  // search for solver
  if (access(SHELL, R_OK ^ X_OK) == -1) {
    fprintf(stderr, "Could not find shell. Exiting\n");
    exit(1);
  }
  
  /* Initialization */
  parseArgs(argc, (char** const)argv);

  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-Client.c
  *
  * =============================================================================
  */
