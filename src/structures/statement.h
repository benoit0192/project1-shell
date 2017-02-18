#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <stdlib.h>

#include "command.h"
#include "assignment.h"
#include "group.h"

#define STATEMENT_TYPE_COMMAND    0
#define STATEMENT_TYPE_ASSIGNMENT 1
#define STATEMENT_TYPE_GROUP      2

union statement_type {
    struct command *command;
    struct assignment *assignment;
    struct group *group;
};

struct statement {
    int type;
    union statement_type statement;
};



void statement__free(struct statement *st);



#endif // _STATEMENT_H_
