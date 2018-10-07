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

enum {
  TRUE = 1,
  FALSE = 0
};

/* command codes */
enum {
  run_code = 0,
  exit_code = 1,
  invalid_code = -1
};

typedef struct {
  int code;
  char * inputfile;
} command_t;

/* get a command from stdin (displays prompt)*/
command_t get_command();

/* get a command from stdin (displays prompt)*/
void print_command(command_t);

#endif // ! __COMMAND_H
