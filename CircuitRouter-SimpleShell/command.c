/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * defines the command datatype
 *
 * a command can be either run (0) or exit (1), with -1 indicating error
 * =============================================================================
 * command.c
 * =============================================================================
 */

/* parse a command from an array of strings with argc tokens */
command_t parse_command(char **argv, int argc)
{
  command_t cmd;
  cmd.code = invalid_code;
  cmd.inputfile = NULL;

  if (strncmp(argv[0], "run", 4) == 0) {
    // if it is a run command, then there are exactly two arguments
    if (argc == 2) {
      cmd.code = run_code;
      cmd.inputfile = argv[1]; // careful, this is a shallow copy
    }
  }
  else if (strncmp(argv[0], "exit", 5) == 0) {
    // if there are more arguments, the command is invalid
    if (argc == 1)
      cmd.code = exit_code;
  }

  return cmd;
}
