#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "errors.h"

struct input_buffer {
    char * prompt;
    char * buff;
    size_t buff_size;
    int pos;
    int old_pos;
};

#include "autocomplete.h"

/**
 * Displays 'prompt' to the user and read his input. Returns the command line
 * that the user typed.
 * the result must be freed by the user
 */
char * input(char * prompt);


#endif //_INPUT_H_
