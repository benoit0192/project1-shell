#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <stdlib.h>
#include "statement.h"


struct sequence {
    struct statement *pipeline; // TODO pipeline
    struct sequence *next;
};


/**
 * append the pipeline 'pipe' at the end of the sequence list 'seq'
 * 'seq' might be NULL for the empty sequence.
 * This function returns the new first element of the sequence. unchanged if the sequence was not empty (NULL).
 */
struct sequence * sequence__append_pipeline(struct sequence *seq, struct statement *pipe);

void sequence__free(struct sequence *seq);

/**
 * Sets background flag for the last command of the sequence
 */
void sequence__set_background(struct sequence *seq, int background);



#endif // _SEQUENCE_H_
