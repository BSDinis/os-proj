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
#include "lib/hashtable.h"

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

simple_list_t global_zombies; // finished processes
hashtable_t global_active; // active processes (limited by global_max_children)

/* =============================================================================
 * compare pid pointers
 * =============================================================================
 */
int compare_pid_t(const void *a, const void *b)
{
  if (a == NULL || b == NULL) return 1;

  const pid_t * first = (const pid_t *) a;
  const pid_t * second = (const pid_t *) b;
  return (*first == *second) ? 1 : 0;
}


/* =============================================================================
 * hash of a pid pointer
 * =============================================================================
 */
ssize_t hash_pid_1(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;

  const pid_t *pid = (const pid_t *) ptr;
  return *pid % cap;
}


/* =============================================================================
 * 2nd hash of a pid pointer
 * =============================================================================
 */
ssize_t hash_pid_2(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;

  const pid_t *pid = (const pid_t *) ptr;
  return *pid % 7 + 1;
}


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
 * add_zombie
 * =================================================================
 */
void add_zombie(child_ctx_t *ctx)
{
  if (simple_list_pushback(global_zombies, (void *) ctx) == -1) 
    fprintf(stderr, "add_zombie: simple_list_pushback returned error\n");
}

/* =================================================================
 * add_active
 * =================================================================
 */
void add_active(pid_t pid)
{
  pid_t *ptr = malloc(sizeof(pid));
  *ptr = pid;
  if (hashtable_add(global_active, (void *) ptr) == -1)
    fprintf(stderr, "add_active: hashtable_add returned error\n");
}

/* =================================================================
 * rem_active
 * =================================================================
 */
void rem_active()
{                
  pid_t pid;
  void *ptr;
  child_ctx_t *finished = malloc(sizeof(child_ctx_t));
  int ret;
  int counter = 0; // we don't want to retry wait indefinetly
  int successful_wait = 0;

  do {
    pid = wait(&ret);
    counter++;
    if (pid != -1) {
      successful_wait = 1;
    }
    else if (errno == ECHILD) {
      successful_wait = 1;
      fprintf(stderr, "rem_active: called wait on childless process.\nAborting\n");
    }
    else if (errno == EINTR) {
      perror("rem_active");
      fprintf(stderr, "Retrying (%d)\n", counter);
    }
  } while (!successful_wait && counter <= MAX_WAIT_RETRIES);

  finished->status_code = ret;

  finished->pid = pid;
  ptr = hashtable_rem(global_active, (void *) &(finished->pid));
  if (ptr != NULL)
    free(ptr);

  add_zombie(finished);
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
      && hashtable_size(global_active) == global_max_children) {
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
  add_active(cid);
}

/* =============================================================================
  * exit_shell: wait for all processes to terminate, print results and exit
  * =============================================================================
  */
static void exit_shell()
{
  while (hashtable_size(global_active) != 0) {
    rem_active();
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


  global_zombies = simple_list_();  

  ssize_t init_capacity = 0;
  if (global_max_children != -1)
    init_capacity = global_max_children;
  else
    init_capacity = 1 << 8;

  global_active = hashtable_(init_capacity,
      hash_pid_1,
      hash_pid_2,
      compare_pid_t);

  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
    free_command(cmd);
  } while (cmd.code != exit_code);

  free_hashtable(global_active);
  free_simple_list(global_zombies);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-SimpleShell.c
  *
  * =============================================================================
  */
