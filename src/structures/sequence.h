#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <stdlib.h>
#include "pipeline.h"

/*---- Sequence ---------------------------------------------------------
*   This structure stores a pipelined command as well as the sequence
*   adjacent to that sequence
*--------------------------------------------------------------------------*/

struct sequence {
    struct pipeline *pipeline;
    struct sequence *next;
};


/**
 * Append the pipeline 'pipe' at the end of the sequence list 'seq'
 * 'seq' might be NULL for the empty sequence.
 * This function returns the new first element of the sequence. unchanged if the sequence was not empty (NULL).
 */
struct sequence * sequence__append_pipeline(struct sequence *seq, struct pipeline *pipe);

/**
 * Append elements of tail at the end of seq.
 * Return the updated sequence.
 */
struct sequence * sequence__append_sequence(struct sequence *seq, struct sequence *tail);

/**
 * Free a sequence structure and all its fields
 */
void sequence__free(struct sequence *seq);

/**
 * Set the 'background' flag to the pipeline of the last sequence in the linked list
 */
void sequence__set_background(struct sequence *seq, int background);

/**
 * Execute a sequence by executing every pipeline in the sequence
 * Return the status of the last pipeline execution
 */
int sequence__execute(struct sequence *seq);

#endif // _SEQUENCE_H_
