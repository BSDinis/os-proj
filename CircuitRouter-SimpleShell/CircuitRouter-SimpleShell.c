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
static int compare_pid_t(const void *a, const void *b)
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
static ssize_t hash_pid_1(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;

  const pid_t *pid = (const pid_t *) ptr;
  return *pid % cap;
}


/* =============================================================================
 * 2nd hash of a pid pointer
 * =============================================================================
 */
static ssize_t hash_pid_2(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;

  const pid_t *pid = (const pid_t *) ptr;
  return *pid % 7 + 1;
}


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
  if (simple_list_pushback(global_zombies, (void *) ctx) == -1)  {
    fprintf(stderr, "add_zombie: simple_list_pushback returned error\n. Aborting.");
    abort();
  }
}

/* =================================================================
 * add_active
 * =================================================================
 */
static void add_active(const pid_t pid)
{
  pid_t *ptr = malloc(sizeof(pid_t));
  if (ptr == NULL) {
    perror("add_active: malloc");
    exit(-2);
  }
  
  *ptr = pid;
  if (hashtable_add(global_active, (void *) ptr) == -1) {
    fprintf(stderr, "add_active: hashtable_add returned error.\nAborting\n");
    abort();
  }
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

  void * ptr = hashtable_rem(global_active, (void *) &(finished->pid));
  if (ptr != NULL)
    free(ptr);

  add_zombie(finished);
}


/* =================================================================
 * print_status: transverse function
 * =================================================================
 */
static int print_status(void *arg)
{
  child_ctx_t *ctx = (child_ctx_t *) arg;
  int success = WIFEXITED(ctx->status_code) && WEXITSTATUS(ctx->status_code) == 0;
  printf("CHILD EXITED (PID=%d; return %sOK)\n", ctx->pid, (success) ? "" : "N");
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
  else if (cid == -1) {
    // error
    perror("run_solver: fork");
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
  FILE *fp = fopen(SOLVER, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not find solver. Exiting\n");
    exit(1);
  }
  fclose(fp);
  
  /* Initialization */
  parseArgs(argc, (char** const)argv);

  global_zombies = simple_list_();  
  if (global_zombies == NULL) {
    fprintf(stderr, "failed to initialize zombie list. aborting\n");
    return -1;
  }
  
  ssize_t init_capacity = 0;
  if (global_max_children != -1)
    init_capacity = global_max_children * 2; 
    /* here we have a performance - memory tradeoff,
     * where time performance is favoured
     *
     * if we define the initial capacity as max_children,
     * once max_children / 2 elements are added, the hashtable is 
     * reallocated and all the elements are inserted again 
     * (which is quite expensive)
     *
     * since we know we will never insert more than global_max_children,
     * by doubling the initial capacity, we avoid that operation
     * entirely */
  else
    init_capacity = 1 << 8;

  global_active = hashtable_(init_capacity,
      hash_pid_1,
      hash_pid_2,
      compare_pid_t);

  if (global_active == NULL) {
    fprintf(stderr, "failed to initialize zombie list. aborting\n");
    free_simple_list(global_zombies);
    return -1;
  }
  
  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
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
