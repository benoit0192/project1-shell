#include "sequence.h"



/**
 * append the pipeline 'pipe' at the end of the sequence list 'seq'
 * 'seq' might be NULL for the empty sequence.
 * This function returns the new first element of the sequence. unchanged if the sequence was not empty (NULL).
 */
struct sequence * sequence__append_pipeline(struct sequence *seq, struct statement *pipe) { // TODO pipeline
    struct sequence *prev    = seq;
    struct sequence *current = seq;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        prev = malloc(sizeof(struct sequence));
        prev->pipeline = pipe;
        prev->next = NULL;
        return prev;
    } else {
        prev->next = malloc(sizeof(struct sequence));
        prev->next->pipeline = pipe;
        prev->next->next = NULL;
        return seq;
    }
}

/**
 * appends elements of tail at the end of seq
 */
struct sequence * sequence__append_sequence(struct sequence *seq, struct sequence *tail) {
    struct sequence *prev    = seq;
    struct sequence *current = seq;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        return tail;
    } else {
        prev->next = tail;
        return seq;
    }
}



void sequence__set_background(struct sequence *seq, int background) {
    if(seq == NULL) return;
    struct sequence *prev    = seq;
    struct sequence *current = seq;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }

    switch (prev->pipeline->type) {
        case STATEMENT_TYPE_COMMAND:
            prev->pipeline->statement.command->background = background;
            break;
        case STATEMENT_TYPE_ASSIGNMENT:
            break;
        case STATEMENT_TYPE_GROUP:
            prev->pipeline->statement.group->background = background;
            break;
        default:
            break;
    }
}


void sequence__free(struct sequence *seq) {
    struct sequence *current = seq;
    struct sequence *next = seq;
    while(current != NULL) {
        next = current->next;
        statement__free(current->pipeline); // TODO pipeline
        free(current);
        current = next;
    }
}
