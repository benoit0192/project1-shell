#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/************************* GLOBAL VARIABLES *************************/

// global variables for Yacc parser
extern FILE *yyin;
extern int yyparse(void);

// file name of the current script beeing read.
// set to "-" if reading from interactive prompt
char *file_name;


/************************* MAIN FUNCTIONS *************************/

// initializes shell, sets glabal variables, reads PROFILE file...
void init_shell() {
    file_name = NULL;

}



// unallocate shell ressources before exiting
void clean_shell() {
    free(file_name);

}



// starts an interactive sheel
void interactive_shell() {
    file_name = strdup("-");
    printf("Let's go interactive!\n");
    // ...
}



// execute a shell script
void exec_script(char *filename) {
    file_name = strdup(filename);
    yyin = fopen (filename, "r");
	if(!yyin) {
	    fprintf(stderr, "Could not open %s\n", filename);
        perror("Error cause: ");
	    return;
	}
    yyparse();
    fclose(yyin);
}



// this is main :)
int main (int argc, char *argv[]) {
    init_shell();
    if(argc >= 2) {
        // execute the script file passed in argument
        exec_script(argv[1]);
    } else {
        // no script files are passed in argument
        // let's start an interactive shell
        interactive_shell();
    }
    clean_shell();
    return 0;
}
