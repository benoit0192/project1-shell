#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * like stdlib.h's malloc but exits with error if malloc was unsuccessful
 */
void * mymalloc(size_t s);

/**
 * like stdlib.h's realloc but exits with error if realloc was unsuccessful
 */
void * myrealloc(void * mem, size_t s);

/**
 * like string.h's strdup but exits with error if strdup was unsuccessful
 */
char * mystrdup(char * s);


#endif // _ERRORS_H_
