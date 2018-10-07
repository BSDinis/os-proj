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

#include "command.h"

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
  * print_command_help: show comand help
  * =============================================================================
  */
static void print_command_help()
{
  fprintf(stderr, "Invalid command entered\n");
  fprintf(stderr, "Command List:\n");
  fprintf(stderr, "run <inputfile>\t: run CircuitSeqSolver for inputfile\n");
  fprintf(stderr, "exit\t\t: exit shell, showing the results for all the processes\n");
}

/* =============================================================================
  * run_solver: spawn a new Circuit Seq Solver as a detached process
  * =============================================================================
  */
static void run_solver(const char *inputfile)
{
  printf("You entered an run command for file %s\n", inputfile);
}

/* =============================================================================
  * exit_shell: wait for all processes to terminate, print results and exit
  * =============================================================================
  */
static void exit_shell()
{
  printf("You entered an exit command\n");
}

/* =============================================================================
  * execute_command: service the client's request
  * =============================================================================
  */
void execute_command(command_t cmd)
{
  switch (cmd.code) {
    case run_code:
      run_solver(cmd.inputfile);
      break;
    case exit_code:
      exit_shell();
      break; 
    case invalid_code:
    default:
      print_command_help();
      break;
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

  command_t cmd;

  do {
    cmd = get_command();
    execute_command(cmd);
    free_command(cmd);
  } while (cmd.code != exit_code);

  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
