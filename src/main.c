#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "input.h"
#include "autocomplete.h"
#include "structures/sequence.h"

/************************* GLOBAL VARIABLES *************************/

// global variables for Yacc parser
extern FILE *yyin;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern void yylex_destroy();

extern int column;
extern int yylineno;

// file name of the current script beeing read.
// set to "-" if reading from interactive prompt
char *script_filename;

// old terminal settings
struct termios old;

// the parsed script to execute. set by the starting rule of yyparse()
struct sequence *script = NULL;

// the exit status of the parser. 0=no error ; 1=error
int parsing_status = 0;

#ifdef UBU // DEBUG on ubuntu
#define DEFAULT_HISTORYFILE "/home/pyvain/shell_history"
#define DEFAULT_PROFILEFILE "/home/pyvain/shell_profile"
#else // MINIX
#define DEFAULT_HISTORYFILE "/root/shell_history"
#define DEFAULT_PROFILEFILE "/root/shell_profile"
#endif


/************************* MAIN FUNCTIONS *************************/

void free_script() {
    sequence__free(script);
    script = NULL;
    parsing_status = 0;
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
    free(script_filename);
    fclose(yyin);
    // reset position in parser
    column = 0;

    // execute command
    int ret;
    if(script != NULL && parsing_status == 0) {
        ret = sequence__execute(script);
    }
    free_script();
}

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

    exec_script(DEFAULT_PROFILEFILE);
}



// unallocate shell ressources before exiting
void clean_shell() {
    free(script_filename);

    // restore initial behavior
    tcsetattr(STDIN_FILENO,TCSANOW,&old);

    history_save(DEFAULT_HISTORYFILE);
    environment_variable__free();

    yylex_destroy();
}



// starts an interactive sheel
void interactive_shell() {
    script_filename = strdup("-");
    char *home=environment_variable__get("HOME");
    cd(home);
    free(home);
    char * cmd;
    while(1) {
        char *prompt;
        char *cwd = getcwd(NULL, 0);
        asprintf(&prompt, "\033[36m%s\033[32m >\033[39m ", cwd);
        free(cwd);
        cmd = input(prompt);
        free(prompt);
        history_push(cmd);
        if(strcmp(cmd,"exit") == 0)
            break;

        // parse command
        YY_BUFFER_STATE buffer = yy_scan_string(cmd);
        yyparse();
        yy_delete_buffer(buffer);
        // reset position in parser
        column = 0;

        // execute command
        int ret;
        if(script != NULL && parsing_status == 0) {
            ret = sequence__execute(script);
        }
        free_script();

        free(cmd);
    }
    free(cmd);
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
