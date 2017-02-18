#ifndef _PIPELINE_H_
#define _PIPELINE_H_

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


#endif // _PIPELINE_H_
