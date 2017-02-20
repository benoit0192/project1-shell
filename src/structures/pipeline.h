#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "statement.h"


struct pipeline {
    struct statement *statement;
    int background;
    struct pipeline *next;
};


/**
 * append the statement 'st' at the end of the pipeline list 'pipe'
 * 'pipe' might be NULL for the empty list.
 * This function returns the new first element of the pipeline. unchanged if the list was not empty (NULL).
 */
struct pipeline * pipeline__append_statement(struct pipeline *pipe, struct statement *st);

void pipeline__free(struct pipeline *p);

int pipeline__execute(struct pipeline *pipeline);


#endif // _PIPELINE_H_
