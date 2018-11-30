/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * defines the command datatype
 *
 * a command can be either run (0) or exit (1), with -1 indicating error
 * =============================================================================
 * command.h
 * =============================================================================
 */

#ifndef __COMMAND_H
#define __COMMAND_H

#define MAXFILENAMESIZE (1<<10) // 1024

/* command codes */
enum {
  run_code = 0,
  exit_code = 1,
  invalid_code = -1
};

typedef struct {
  int code;
  char inputfile[MAXFILENAMESIZE+1];
  int has_inputfile;
  char *pipe_name;
} command_t;

/* get a command from input stream (displays prompt)*/
command_t get_command(FILE *is);

/* print a command (debug)*/
void print_command(command_t);

#endif // ! __COMMAND_H
