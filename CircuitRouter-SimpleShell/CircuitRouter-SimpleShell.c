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
#include "lib/vector.h"

#define SOLVER "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver"

// when removing an active child (by waiting) the wait sys call may fail
// this defines how many times it is retried
#define MAX_WAIT_RETRIES 5

/* 
 * child_ctx_t
 * hold relevant information related to a child process
 */
typedef struct {
  pid_t pid;
  int status_code;
} child_ctx_t;

int64_t global_max_children;

vector_t *global_zombies; // finished processes
int32_t global_n_active; // active processes (limited by global_max_children)



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
  global_max_children = -1;
  global_n_active = 0;
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
 * add_zombie
 * =================================================================
 */
static void add_zombie(child_ctx_t * ctx)
{
  if (vector_pushBack(global_zombies, (void *) ctx) == FALSE)  {
    fprintf(stderr, "add_zombie: vector_pushBack returned error\n. Aborting.");
    abort();
  }
}

/* =================================================================
 * add_active
 * =================================================================
 */
static void add_active()
{
  global_n_active++;
}

/* =================================================================
 * rem_active
 * =================================================================
 */
static void rem_active()
{                
  pid_t pid;
  int status;
  int counter = 0; // we don't want to retry wait indefinetly
  int successful_wait = 0; // flag to know when to exit the cycle

  do {
    pid = wait(&status);
    counter++;
    int _errno = errno; // push errno
    errno = 0;
    if (pid != -1) {
      successful_wait = 1;
    }
    else if (errno == ECHILD) {
      successful_wait = 1;
      fprintf(stderr, "rem_active: called wait on childless process.\nAborting\n");
      abort();
    }
    else if (errno == EINTR) {
      perror("rem_active");
      fprintf(stderr, "Retrying (%d)\n", counter);
    }
    errno = _errno; // pop errno
  } while (!successful_wait && counter <= MAX_WAIT_RETRIES);

  child_ctx_t *finished = malloc(sizeof(child_ctx_t));
  if (finished == NULL) {
    perror("rem_active: malloc");
    exit(-2);
  }
  
  finished->status_code = status;
  finished->pid = pid;

  add_zombie(finished);
  global_n_active--;
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
      && global_n_active == global_max_children) {
    rem_active();
  }
  
  pid_t cid = fork();
  if (cid == 0) {
    // child
    if (execl(SOLVER, SOLVER, inputfile, (char *) NULL) == -1) {
      perror("FATAL ERROR: run_solver: execl");
      exit(-2);
    }
  }
  else if (cid == -1) {
    // error
    perror("run_solver: fork");
  }
  
  add_active();
}

/* =============================================================================
  * exit_shell: wait for all processes to terminate, print results and exit
  * =============================================================================
  */
static void exit_shell()
{
  while (global_n_active != 0) {
    rem_active();
  }
  
  ssize_t size = vector_getSize(global_zombies);
  for (ssize_t i = 0; i < size; i++) {
    void * data = vector_at(global_zombies, i);
    if (data == NULL) continue;

    child_ctx_t * ctx = (child_ctx_t * ) data;
    int success = WIFEXITED(ctx->status_code) && WEXITSTATUS(ctx->status_code) == 0;
    fprintf(stdout, "CHILD EXITED (PID=%d; return %sOK)\n", ctx->pid, (success) ? "" : "N");
    free(ctx);
  }
  
  printf("END.\n");
}

/* =============================================================================
  * execute_command: service the client's request
  * =============================================================================
  */
static void execute_command(const command_t cmd)
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
  if (access(SOLVER, R_OK ^ X_OK) == -1) {
    fprintf(stderr, "Could not find solver. Exiting\n");
    exit(1);
  }
  
  /* Initialization */
  parseArgs(argc, (char** const)argv);

  global_zombies = vector_alloc(global_max_children);
  if (global_zombies == NULL) {
    fprintf(stderr, "failed to initialize zombie list. aborting\n");
    return -1;
  }
  
  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
  } while (cmd.code != exit_code);

  vector_free(global_zombies);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
