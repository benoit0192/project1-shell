#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "input.h"

/************************* GLOBAL VARIABLES *************************/

// global variables for Yacc parser
extern FILE *yyin;
extern int yyparse(void);

// file name of the current script beeing read.
// set to "-" if reading from interactive prompt
char *script_filename;

// old terminal settings
struct termios old;


/************************* MAIN FUNCTIONS *************************/

// initializes shell, sets glabal variables, reads PROFILE file...
void init_shell() {
    script_filename = NULL;

    /* initialize unbuffered terminal */
    struct termios new;
    tcgetattr(STDIN_FILENO,&old);         /* read terminal attributes */
    new=old;                              /* get old settings */
    new.c_lflag &=(~ICANON & ~ECHO);      /* modify the current settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&new); /* push the settings on the terminal */
}



// unallocate shell ressources before exiting
void clean_shell() {
    free(script_filename);

    /* ok, restore initial behavior */
    tcsetattr(STDIN_FILENO,TCSANOW,&old);
}



// starts an interactive sheel
void interactive_shell() {
    script_filename = strdup("-");
    printf("Let's go interactive!\n");
    char * cmd = "";
    while(strcmp(cmd,"exit") != 0) {
        cmd = input("> ");
    }
}



// execute a shell script
void exec_script(char *filename) {
    script_filename = strdup(filename);
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
