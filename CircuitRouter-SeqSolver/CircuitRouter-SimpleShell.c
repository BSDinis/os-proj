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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint64_t global_max_children = -1; // since this is unsigned it becomes the maximum

/* =============================================================================
  * displayUsage
  * =============================================================================
  */
static void display_usage(const char* program_name){
  printf("Usage: %s <max_children> (default: unlimited)\n", program_name);
  exit(1);
}


/* =============================================================================
  * parseArgs
  * =============================================================================
  */
static void parseArgs (long argc, char* const argv[]){
  
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


  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
