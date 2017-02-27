#ifndef _AUTOCOMPLETE_H_
#define _AUTOCOMPLETE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "input.h"

/**
 * loads the history file 'filename'
 */
void history_load(char * filename);

/**
 * saves the current history to the file 'filename'
 */
void history_save(char * filename);

/**
 * Adds new element to history and reset current suggestion and autocomplete parameter
 */
void history_push(char * value);


/**
 * Move back in time in the history
 * Returns a new autocomplete suggestion.
 * the result must be freed by the user
 */
char * history_prev(struct input_buffer * inbuff);

/**
 * Move forward in time in the history
 * Returns a new autocomplete suggestion.
 * the result must be freed by the user
 */
char * history_next(struct input_buffer * inbuff);

/**
 * Sets current autocomplete parameters, set current suggestion to the first available one and returns it
 */
char * history_match(struct input_buffer * inbuff);



#endif // _AUTOCOMPLETE_H_
