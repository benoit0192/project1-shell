#include "sequence.h"



/**
 * Append the pipeline 'pipe' at the end of the sequence list 'seq'
 * 'seq' might be NULL for the empty sequence.
 * This function returns the new first element of the sequence. unchanged if the sequence was not empty (NULL).
 */
struct sequence * sequence__append_pipeline(struct sequence *seq, struct pipeline *pipe) {
    struct sequence *prev    = seq;
    struct sequence *current = seq;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        prev = mymalloc(sizeof(struct sequence));
        prev->pipeline = pipe;
        prev->next = NULL;
        return prev;
    } else {
        prev->next = mymalloc(sizeof(struct sequence));
        prev->next->pipeline = pipe;
        prev->next->next = NULL;
        return seq;
    }
}

/**
 * Append elements of tail at the end of seq.
 * Return the updated sequence.
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

/**
 * Set the 'background' flag to the pipeline of the last sequence in the linked list
 */
void sequence__set_background(struct sequence *seq, int background) {
    if(seq == NULL) return;
    struct sequence *prev    = seq;
    struct sequence *current = seq;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    prev->pipeline->background = background;
}

/**
 * Free a sequence structure and all its fields
 */
void sequence__free(struct sequence *seq) {
    struct sequence *current = seq;
    struct sequence *next = seq;
    while(current != NULL) {
        next = current->next;
        pipeline__free(current->pipeline);
        free(current);
        current = next;
    }
}

/**
 * Execute a sequence by executing every pipeline in the sequence
 * Return the status of the last pipeline execution
 */
int sequence__execute(struct sequence *seq) {
    int ret;
    struct sequence *current = seq;
    while(current != NULL) {
        ret = pipeline__execute(current->pipeline);
        current = current->next;
    }

    return ret;
}
