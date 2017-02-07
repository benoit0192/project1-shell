#ifndef _COMMAND_H_
#define _COMMAND_H_


/*---- Command ------------------------------------------------------------
*   This structure is used to represent a simple command.
*   It contains
*      - the program name as specified in input (not extended with $PATH)
*      - the list of its arguments
*      - an integer specifying if the command must be run in background
*      - its input file descriptor
*      - its ouput file descriptor
*--------------------------------------------------------------------------*/

struct command {
    char * prog_name;
    /*arg_list*/void * args;
    int background;
    int in;
    int out;
};




#endif // _COMMAND_H_
