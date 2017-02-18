#include "pipeline.h"


/**
 * append the statement 'st' at the end of the pipeline list 'pipe'
 * 'pipe' might be NULL for the empty list.
 * This function returns the new first element of the pipeline. unchanged if the list was not empty (NULL).
 */
struct pipeline * pipeline__append_statement(struct pipeline *pipe, struct statement *st) {
    struct pipeline *prev    = pipe;
    struct pipeline *current = pipe;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        prev = malloc(sizeof(struct pipeline));
        prev->statement = st;
        prev->next = NULL;
        return prev;
    } else {
        prev->next = malloc(sizeof(struct pipeline));
        prev->next->statement = st;
        prev->next->next = NULL;
        return pipe;
    }
}


void pipeline__free(struct pipeline *p) {
    struct pipeline *current = p;
    struct pipeline *next = p;
    while(current != NULL) {
        next = current->next;
        statement__free(current->statement);
        free(current);
        current = next;
    }
}
