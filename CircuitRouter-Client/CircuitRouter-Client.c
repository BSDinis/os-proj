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
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE (1 << 10)
#define PIPE_NAME_SZ (1 << 8)

char * pipe_pathname = NULL;

/* =============================================================================
  * displayUsage
  * =============================================================================
  */
static void display_usage(const char * program_name){
  printf("Usage: %s <input pipe pathname>\n", program_name);
  exit(1);
}

/* =============================================================================
  * parseArgs
  * =============================================================================
  */
static void parseArgs (const long argc, char* const argv[]){
  if (argc != 2) {
    display_usage(argv[0]);
  }

  pipe_pathname = argv[1];
  if (access(pipe_pathname, W_OK) == -1) {
    fprintf(stderr, "Could not find shell input pipe. Exiting\n");
    display_usage(argv[0]);
  }
}

/* =============================================================================
  * main
  * =============================================================================
  */
int main(int argc, char** argv){
  /* Initialization */
  parseArgs(argc, (char** const)argv);
  char input_pipe_name[PIPE_NAME_SZ];
  snprintf(input_pipe_name, PIPE_NAME_SZ - 1,  "/tmp/CircuitRouter-Client.pipe.%x", getpid());
  input_pipe_name[PIPE_NAME_SZ - 1] = '\0';
  mkfifo(input_pipe_name, 0666);

  FILE * output_pipe = fopen(pipe_pathname, "w+");
  FILE * input_pipe = fopen(input_pipe_name, "w+");

  ssize_t cmd_buffersz = strlen(input_pipe_name) + BUFFER_SIZE + 3;
  char cmd_buffer[cmd_buffersz];
  char buffer[BUFFER_SIZE];
  while (1) {      
    fputs("> ", stdout);
    if (fgets(buffer, BUFFER_SIZE - 1, stdin) == NULL)
      break;
    buffer[BUFFER_SIZE - 1] = '\0';

    snprintf(cmd_buffer, cmd_buffersz, "%s %s", input_pipe_name, buffer);
    fputs(cmd_buffer, output_pipe);
    fflush(output_pipe); 
    
    fgets(buffer, BUFFER_SIZE - 1, input_pipe);
    buffer[BUFFER_SIZE - 1] = '\0';
    fputs(buffer, stdout);
  }

  fclose(output_pipe);
  fclose(input_pipe);
  exit(0);
}


/* =============================================================================
  *
  * End of CircuitRouter-Client.c
  *
  * =============================================================================
  */
