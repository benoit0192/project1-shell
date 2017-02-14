#ifndef _AUTOCOMPLETE_H_
#define _AUTOCOMPLETE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void history_load(char * filename);

void history_save(char * filename);

/**
 * Adds new element to history and reset current suggestion and autocomplete parameter
 */
void history_push(char * value);

char * history_prev(char * start_string);

char * history_next(char * start_string);

/**
 * Sets current autocomplete parameters, set current suggestion to the first available one and returns it
 */
char * history_match(char * start_string);



#endif // _AUTOCOMPLETE_H_
