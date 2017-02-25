#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "statement.h"

/*---- Pipeline ---------------------------------------------------------
*   This structure stores a statement to execute,
*	a variable for running the piped statement in background or not and
*	a variable for the adjacent piped statement
*--------------------------------------------------------------------------*/

struct pipeline {
    struct statement *statement;
    int background;
    struct pipeline *next;
};


/**
 * Append the statement 'st' at the end of the pipeline list 'pipe'
 * 'pipe' might be NULL for the empty list.
 * This function returns the new first element of the pipeline. unchanged if the list was not empty (NULL).
 */
struct pipeline * pipeline__append_statement(struct pipeline *pipe, struct statement *st);

/**
 * Free a pipeline structure and all its contents
 */
void pipeline__free(struct pipeline *p);

/**
 * Execute a pipeline by executing each statement piped into another one
 * If a pipeline is not executed in background, set the alarm for 5s if enabled then return the status of the last pipeline statement.
 * Otherwise return 0.
 */
int pipeline__execute(struct pipeline *pipeline);


#endif // _PIPELINE_H_
