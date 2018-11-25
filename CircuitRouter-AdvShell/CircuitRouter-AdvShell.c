/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Adv Shell to spawn CircuitSolver-ParSolver processes
 * and receive remote requests
 *
 * Interface
 * 
 * run filename (solve the Circuit at that filename)
 * ---exit (exit the application, printing all the results from the processes)
 * =============================================================================
 *
 * CircuitRouter-AdvShell.c
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

#include <signal.h>
#include <time.h>

#include "command.h"
#include "lib/vector.h"
#include "lib/hashtable.h"	

#define SOLVER "../CircuitRouter-ParSolver/CircuitRouter-ParSolver"
#define DEFAULT_PARALELISM "8"

// when removing an active child (by waiting) the wait sys call may fail
// this defines how many times it is retried
#define MAX_WAIT_RETRIES 5

/* 
 * child_ctx_t
 * hold relevant information related to a child process
 */
typedef struct {
  pid_t pid;
  long sec;
  struct timespec begin;
  int status;
  bool ok;
} child_ctx_t;

int64_t global_max_children;

vector_t *global_zombies; // finished processes
hashtable_t global_active; // active processes (limited by global_max_children)		￼

/* =============================================================================	
 * compare child ctx pointers	
 * =============================================================================	
 */	
static int compare_ctx_t(const void *a, const void *b)	
{	
  if (a == NULL || b == NULL) return 1;	

  const pid_t * first = &((const child_ctx_t *) a)->pid;	
  const pid_t * second = &((const child_ctx_t *) b)->pid;	
  return (*first == *second) ? 1 : 0;	
}

/*=============================================================================
 * hash of a pid pointer
 * =============================================================================
 */
static ssize_t hash_pid_1(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;

  const pid_t *pid = &((const child_ctx_t *)ptr)->pid;
  return *pid % cap;
}

/* =============================================================================
 * 2nd hash of a pid pointer
 * =============================================================================
 */
static ssize_t hash_pid_2(const ssize_t cap, const void * ptr)
{
  if (cap <= 0 || ptr == NULL) return 1;
  const pid_t *pid = &((const child_ctx_t *)ptr)->pid;
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
  if (vector_pushBack(global_zombies, (void *) ctx) == FALSE)  {
    fprintf(stderr, "add_zombie: vector_pushBack returned error\n. Aborting.");
    abort();
  }
}

/* =================================================================
 * add_active
 * =================================================================
 */
static void add_active(const pid_t pid)
{
  child_ctx_t *ptr = malloc(sizeof(child_ctx_t));
  if (ptr == NULL) {
    perror("add_active: malloc");
    exit(-2);
  }

  if (clock_gettime(CLOCK_REALTIME, &ptr->begin) == -1) {
    perror("add_active: clock_gettime");
    exit(-1);
  }
  ptr->pid = pid;
  ptr->sec = 0;
  ptr->status = -1;
  ptr->ok = false;
  if (hashtable_add(global_active, (void *) ptr) == -1) {
    fprintf(stderr, "add_active: hashtable_add returned error.\nAborting\n");
    abort();
  }
}

static long get_secs(const struct timespec *a, const struct timespec *b)
{
  long sec = a->tv_sec - b->tv_sec;
  long nsec = a->tv_nsec - b->tv_nsec;
  if (nsec > 500000000) sec++;
  else if (nsec < -500000000) sec--;
  return sec;
}


/* =================================================================
 * rem_active: treat SIGCHLD signal
 * =================================================================
 */
static void rem_active(int signo, siginfo_t *info, void *uctx)
{                
  if (signo != SIGCHLD) return;
  
  pid_t pid;
  int status = 0;

  // waitpid allows for WNOHANG; this way concorrent 
  // children exiting get caught
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (!pid) continue;
    if (pid == -1) {
      perror("rem_active: waitpid");
      exit(-1);
    }
    
    struct timespec end;
    if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
      exit(-1); // FIXME - no error message - reentrancy
    }
    
    child_ctx_t *finished = hashtable_rem(global_active, (void *)&pid);
    if (finished == NULL) {
      perror("rem_active: hashtable lookup failed");
      exit(-2); // this may need to be removed if not all children are solvers
    }
    finished->status = WEXITSTATUS(status);
    finished->ok = WIFEXITED(status) && finished->status == 0;
    finished->sec = get_secs(&end, &finished->begin);
    add_zombie(finished);
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
  if (global_max_children != -1 && hashtable_size(global_active) == global_max_children) 
    pause();
  
  pid_t cid = fork();
  if (cid == 0) {
    // child
    if (execl(SOLVER, SOLVER, "-t", DEFAULT_PARALELISM, inputfile, (char *) NULL) == -1) {
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
  while (hashtable_size(global_active) != 0)
    pause();

  ssize_t size = vector_getSize(global_zombies);
  for (ssize_t i = 0; i < size; i++) {
    void * data = vector_at(global_zombies, i);
    if (data == NULL) continue;

    child_ctx_t * ctx = (child_ctx_t * ) data;
    fprintf(stdout, "CHILD EXITED (PID=%d; return %sOK; %ld s)\n", ctx->pid, (ctx->ok) ? "" : "N", ctx->sec); // FIXME times
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

void install_handler(void (*handler)(int, siginfo_t *, void *), int signo)
{
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, signo);

  struct sigaction action;
  action.sa_sigaction = handler;
  action.sa_mask = mask;
  action.sa_flags = SA_NOCLDSTOP; // nevermind stopped children

  sigaction(signo, &action, NULL);
}


/* =============================================================================
  * main
  * =============================================================================
  */
int main(int argc, char** argv){
  install_handler(rem_active, SIGCHLD);
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

  ssize_t init_capacity = 0;
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
     * entirely
     */
  if (global_max_children != -1)
    init_capacity = global_max_children * 2;
  else
    init_capacity = 1 << 8;

   global_active = hashtable_(init_capacity,
      hash_pid_1,
      hash_pid_2,
      compare_ctx_t);

   if (global_active == NULL) {
    fprintf(stderr, "failed to initialize active list. aborting\n");
    vector_free(global_zombies);
    return -1;
  }
  
  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
  } while (cmd.code != exit_code);

  free_hashtable(global_active);
  vector_free(global_zombies);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-AdvShell.c
  *
  * =============================================================================
  */
