/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * defines the command datatype
 *
 * a command can be either run (0) or exit (1), with -1 indicating error
 * =============================================================================
 * command.c
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "lib/commandlinereader.h"
#include "lib/list.h"

#define MAXLINESIZE (1<<10) // 1024


/** 
 * parse a command from an array of strings with argc tokens 
 *
 * @param argv vector with the command and arguments
 * @param argc numver of tokens in argv
 */
static command_t parse_command(char **argv, int argc)
{
  command_t cmd;
  cmd.code = invalid_code;
  cmd.has_inputfile = 0;

  if (argv == NULL || argc <= 0)
    return cmd;

  if (strncmp(argv[0], "run", 4) == 0) {
    // if it is a run command, then there are exactly two arguments
    if (argc == 2) {
      size_t len = strlen(argv[1]);
      len = (len > MAXFILENAMESIZE) ? MAXFILENAMESIZE : len;
      cmd.code = run_code;
      cmd.has_inputfile = 1;
      strncpy(cmd.inputfile, argv[1], len + 1);
    }
  }
  else if (strncmp(argv[0], "exit", 5) == 0) {
    // if there are more arguments, the command is invalid
    if (argc == 1)
      cmd.code = exit_code;
  }

  return cmd;
}


/** 
 * read and parse a command from the shell
 * @return     command
 */
command_t get_command()
{
  command_t cmd;
  int argc;
  char buffer[MAXLINESIZE + 1] = { 0 };
  int max_argc = 2; // run has two args
  char *argv[max_argc + 1];
  
  argc = readLineArguments(argv, max_argc + 1, buffer, MAXLINESIZE);
  if (argc < 0) {
    fprintf(stderr, "get_command: error reading from the line\n");
    exit(2);
  }

  cmd = parse_command(argv, argc);

  return cmd;
}


/**
 * print the command
 * for debug
 */
void print_command(command_t cmd)
{
  switch (cmd.code) {
    case invalid_code:
      printf("invalid command\n");
      break;
    case exit_code:
      printf("exit command\n");
      break;
    case run_code:
      printf("run command; inputfile: %s\n", cmd.inputfile);
      break;
    default:
      printf("unknown command\n");
      break;
  }
}

