/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * defines the command datatype
 *
 * a command can be either run (0) or exit (1), with -1 indicating error
 * =============================================================================
 * command.h
 * =============================================================================
 */

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

/* parse a command from an array of strings with argc tokens */
command_t parse_command(char **argv, int argc);
