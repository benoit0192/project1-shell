#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "input.h"
#include "autocomplete.h"

/************************* GLOBAL VARIABLES *************************/

// global variables for Yacc parser
extern FILE *yyin;
extern int yyparse(void);

// file name of the current script beeing read.
// set to "-" if reading from interactive prompt
char *script_filename;

// old terminal settings
struct termios old;

#define DEFAULT_HISTORYFILE "/root/shell_history"



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

    history_load(DEFAULT_HISTORYFILE);
}



// unallocate shell ressources before exiting
void clean_shell() {
    free(script_filename);

    /* ok, restore initial behavior */
    tcsetattr(STDIN_FILENO,TCSANOW,&old);

    history_save(DEFAULT_HISTORYFILE);
}



// starts an interactive sheel
void interactive_shell() {
    script_filename = strdup("-");
    printf("Let's go interactive!\n");
    char * cmd;
    while(1) {
        cmd = input("\033[36m/root/ \033[32m>\033[39m ");
        history_push(cmd);
        if(strcmp(cmd,"exit") == 0)
            break;

        // parse command
        int fd[2];
        pipe(fd);
        yyin = fdopen(fd[1], "r");
        printf("%d", fd[1]);
        write(fd[0], cmd, strlen(cmd));
        close(fd[0]);
        yyparse();
        fclose(yyin);
        close(fd[1]);

        free(cmd);
    }
    free(cmd);
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
