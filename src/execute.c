#include "execute.h"


// the parsed script to execute. set by the starting rule of yyparse()
struct sequence *script = NULL;

// the exit status of the parser. 0=no error ; 1=error
int parsing_status = 0;


void free_script() {
    sequence__free(script);
    script = NULL;
    parsing_status = 0;
}

void execute_script() {
    
}
