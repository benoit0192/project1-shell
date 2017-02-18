#ifndef _AUTOCOMPLETE_H_
#define _AUTOCOMPLETE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

void history_load(char * filename);

void history_save(char * filename);

/**
 * Adds new element to history and reset current suggestion and autocomplete parameter
 */
void history_push(char * value);

char * history_prev(struct input_buffer * inbuff);

char * history_next(struct input_buffer * inbuff);

/**
 * Sets current autocomplete parameters, set current suggestion to the first available one and returns it
 */
char * history_match(struct input_buffer * inbuff);



#endif // _AUTOCOMPLETE_H_
