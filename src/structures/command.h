#ifndef _COMMAND_H_
#define _COMMAND_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "../errors.h"

/*---- Command ------------------------------------------------------------
*   This structure is used to represent a simple command.
*   It contains
*      - the program name as specified in input (not extended with $PATH)
*      - the list of its arguments
*--------------------------------------------------------------------------*/

struct arg_list {
    char * arg;
    struct arg_list * next;
};

struct command {
    char * prog_name;
    struct arg_list * args;
};

#include "../builtin/cd.h"
#include "../builtin/exit.h"

/**
 * Allocate a new command structure and initialize its elements
 */
struct command* command__new();

/**
 * Append the argument 'arg' at the end of the argument list of 'cmd'
 * the argument should already have been duplicated with strdup()
 */
void command__append_arg(struct command *cmd, char *arg);

/**
 * Free a command structure and all its contents
 */
void command__free(struct command *cmd);

/**
 *  Execute a command.
 *  A command can execute a program either from its absolute or relative path.
 *  Such programms are defined in the environment variable $PATH
 */
int command__execute(struct command *cmd);

#endif // _COMMAND_H_
