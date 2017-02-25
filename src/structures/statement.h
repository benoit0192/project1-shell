#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <stdlib.h>

#include "command.h"
#include "assignment.h"
#include "group.h"

#define STATEMENT_TYPE_COMMAND    0
#define STATEMENT_TYPE_ASSIGNMENT 1
#define STATEMENT_TYPE_GROUP      2

/*---- Statement ---------------------------------------------------------
*   This structure stores the type of the statement that can be either 
*	a command, an assingment or a group.
*	Statement structure contains as well the actual structure associated to
*	the type.
*--------------------------------------------------------------------------*/

union statement_type {
    struct command *command;
    struct assignment *assignment;
    struct group *group;
};

struct statement {
    int type;
    union statement_type statement;
};

/**
 * Free a statement structure and all its fields depending on the type of the statement (command, assignement or group)
 */
void statement__free(struct statement *st);

/**
 * Execute a statement by calling the execution function of its 'statement_type' attribute that
 * match the type of the attribute (command, assignment or group) and return the return value of the execution function
 * If no match has been found, return -1
 */
int statement__execute(struct statement *st);


#endif // _STATEMENT_H_
