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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "lib/simple_list.h"

#define SOLVER "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver"

/* 
 * child_ctx_t
 * hold relevant information related to a child process
 */
typedef struct {
  pid_t pid;
  int status_code;
} child_ctx_t;


int64_t global_max_children;


simple_list_t global_zombies; // finished processes

pid_t * global_active; // active processes (limited by global_max_children)
ssize_t allocd_active;
ssize_t n_active;


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

/* =================================================================
 * add_active
 * =================================================================
 */
void add_active(pid_t pid)
{
  if (n_active == allocd_active) {
    allocd_active *= 2;
    global_active = realloc(global_active, allocd_active * sizeof(pid_t));
  }
  
  global_active[n_active++] = pid;
}

/* =================================================================
 * rem_active
 * =================================================================
 */
void rem_active(pid_t pid)
{
  ssize_t i, j;
  for (i = j = 0; i < n_active; i++) {
    if (global_active[i] == pid)
      continue;

    global_active[j++] = global_active[i];
  }
  
  n_active = j;
}

/* =================================================================
 * add_zombie
 * =================================================================
 */
void add_zombie(child_ctx_t *ctx)
{
  simple_list_pushback(global_zombies, (void *) ctx);
}

/* =================================================================
 * print_status: transverse function
 * =================================================================
 */
int print_status(void *arg)
{
  child_ctx_t *ctx = (child_ctx_t *) arg;
  printf("CHILD EXITED (PID=%d; return %sOK)\n",
      ctx->pid,
      (ctx->status_code == 0) ? "" : "N");

  free(ctx);
  return 0;
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
  if (global_max_children != -1 
      && n_active == global_max_children) {

    child_ctx_t *finished = malloc(sizeof(child_ctx_t));
    int ret;
    finished->pid = wait(&ret);
    finished->status_code = ret;
    rem_active(finished->pid);
    add_zombie(finished);
  }
  
  pid_t cid = fork();
  if (cid == 0) {
    // child
    if (execl(SOLVER, SOLVER, inputfile, (char *) NULL) == -1) {
      perror("FATAL ERROR: run_solver: execl");
      exit(-2);
    }
  }
  add_active(cid);
}

/* =============================================================================
  * exit_shell: wait for all processes to terminate, print results and exit
  * =============================================================================
  */
static void exit_shell()
{
  while (n_active != 0) {
    child_ctx_t *finished = malloc(sizeof(child_ctx_t));
    finished->pid = wait(&(finished->status_code));
    rem_active(finished->pid);
    add_zombie(finished);
  }
  
  simple_list_transverse(global_zombies, print_status);
  printf("END.\n");
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
  // search for solver
  FILE *fp = fopen(SOLVER, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not find solver. Exiting\n");
    exit(1);
  }
  fclose(fp);
  
  /* Initialization */
  parseArgs(argc, (char** const)argv);

  if (global_max_children != -1)
    allocd_active = global_max_children;
  else
    allocd_active = 1 << 6;

  global_zombies = simple_list_();  
  global_active = malloc(allocd_active * sizeof(pid_t));

  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
    free_command(cmd);
  } while (cmd.code != exit_code);

  free(global_active);
  free_simple_list(global_zombies);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
