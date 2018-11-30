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
#include <string.h>
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

/* 
 * child_ctx_t
 * hold relevant information related to a child process
 */
typedef struct {
  pid_t pid;
  long sec;
  struct timespec begin;
  int status;
} child_ctx_t;

int64_t global_max_children;
volatile sig_atomic_t global_active;
vector_t *global_proc; // processes

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
  global_active = 0;
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
    global_max_children = strtoll(argv[1], NULL, 10);
    if (errno != 0) {
      perror("parseArgs: strtoll");
      display_usage(argv[0]);
    }
    else if (global_max_children <= 0) {
      display_usage(argv[0]);
    }
  }
  else {
    setDefaults();
  }
}

/* =================================================================
 * add_process
 * =================================================================
 */
static void add_process(const pid_t pid)
{
  child_ctx_t *ptr = malloc(sizeof(child_ctx_t));
  if (ptr == NULL) {
    perror("add_process: malloc");
    exit(-2);
  }

  if (clock_gettime(CLOCK_REALTIME, &ptr->begin) == -1) {
    perror("add_process: clock_gettime");
    exit(-1);
  }
  ptr->pid = pid;
  ptr->sec = -1;
  ptr->status = -1;
  if (vector_pushBack(global_proc, (void *) ptr) == FALSE)  {
    fprintf(stderr, "add_process: vector_pushBack returned error\n. Aborting.");
    abort();
  }
  global_active++;
}

static long get_secs(const struct timespec *a, const struct timespec *b)
{
  long sec = a->tv_sec - b->tv_sec;
  long nsec = a->tv_nsec - b->tv_nsec;
  if (nsec > 500000000) sec++;
  else if (nsec < -500000000) sec--;
  return sec;
}


static child_ctx_t * find_ctx(vector_t *vec_ptr, pid_t pid)
{      
  if (vec_ptr == NULL) return NULL;
  long size = vector_getSize(vec_ptr);
  for (long i = 0; i < size; i++) { 
    child_ctx_t * ptr = (child_ctx_t *) vector_at(vec_ptr, i);
    if (ptr->pid == pid) return ptr;
  }

  return NULL;
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
      write(2, "SIGCHLD handler: clock_gettime failed\n", strlen("SIGCHLD handler: clock_gettime failed\n")+1);
      exit(-1); 
    }
    
    
    child_ctx_t *finished = find_ctx(global_proc, pid);
    if (finished == NULL) {
      write(2, "rem_active: lookup failed\n", strlen("rem_active: lookup failed\n")+1);
      exit(-2); // this may need to be removed if not all children are solvers
    }

    finished->status = status;
    finished->sec = get_secs(&end, &finished->begin);
    global_active--;
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
  if (global_max_children != -1 && global_active == global_max_children) 
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
  
  add_process(cid);
}

/* =============================================================================
  * exit_shell: wait for all processes to terminate, print results and exit
  * =============================================================================
  */
static void exit_shell()
{
  while (global_active != 0)
    pause();

  ssize_t size = vector_getSize(global_proc);
  for (ssize_t i = 0; i < size; i++) {
    void * data = vector_at(global_proc, i);
    if (data == NULL) continue;

    child_ctx_t * ctx = (child_ctx_t * ) data;
    fprintf(stdout, "CHILD EXITED (PID=%d; return %sOK; %ld s)\n", 
        ctx->pid, 
        (WIFEXITED(ctx->status) && WEXITSTATUS(ctx->status) == 0) ? "" : "N",
        ctx->sec);

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

  global_proc = vector_alloc(global_max_children);
  if (global_proc == NULL) {
    fprintf(stderr, "failed to initialize proc list. aborting\n");
    return -1;
  }

  command_t cmd;
  do {
    cmd = get_command();
    execute_command(cmd);
  } while (cmd.code != exit_code);

  vector_free(global_proc);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-AdvShell.c
  *
  * =============================================================================
  */
