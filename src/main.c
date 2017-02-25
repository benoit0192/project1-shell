#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

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

// main process PID
pid_t main_pid;

// old terminal settings
struct termios old;

// the parsed script to execute. set by the starting rule of yyparse()
struct sequence *script = NULL;

// the exit status of the parser. 0=no error ; 1=error
int parsing_status = 0;

extern volatile int rewind_cursor;

#define DEFAULT_HISTORYFILE "/root/shell_history"
#define DEFAULT_PROFILEFILE "/root/shell_profile"


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
        fprintf(stderr, "Could not open %s: %s\n", filename, strerror(errno));
        free(script_filename);
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

void signalHandlerInt(int sig) {
    printf("\nAll child processes were killed\n");
    write(STDIN_FILENO, "\0", 1); // refresh prompt
    rewind_cursor = 0;
    //fprintf(stdin, "\033[B");
    //fflush(stdin);
}

//void signalHandlerAlarm(int sig);
void signalHandlerAlarm(int sig) {
    signal(SIGALRM, SIG_IGN);
    if(getpid() == main_pid)
        printf("\033[34mINFO\033[39m A command is long to execute. You can press Ctrl-C to kill all child processes.\n");
    signal(SIGALRM, signalHandlerAlarm);
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

    main_pid = getpid();
    signal(SIGALRM, signalHandlerAlarm);

    if(1) {
        exec_script(DEFAULT_PROFILEFILE);
    } else {
        fprintf(stderr, "\033[33mWARNING\033[39m No profile file found (%s). $PATH, $HOME and $ALARM are not set.\n", DEFAULT_PROFILEFILE);
    }

    history_load(DEFAULT_HISTORYFILE);
}

// unallocate shell ressources before exiting
void clean_shell() {
    if(getpid() == main_pid) {
        tcsetattr(STDIN_FILENO,TCSANOW,&old); // restore initial behavior
        history_save(DEFAULT_HISTORYFILE);
    }
    free(script_filename);
    environment_variable__free();
    free_script();
    yylex_destroy();
}



// starts an interactive sheel
void interactive_shell() {
    script_filename = strdup("-");
    cd(NULL); // cd to HOME
    char * cmd;
    while(1) {
        char *prompt;
        char *cwd = getcwd(NULL, 0);
        asprintf(&prompt, "\033[36m%s\033[32m >\033[39m ", cwd);
        free(cwd);
        cmd = input(prompt);
        free(prompt);
        history_push(cmd);

        // parse command
        YY_BUFFER_STATE buffer = yy_scan_string(cmd);
        yyparse();
        yy_delete_buffer(buffer);
        // reset position in parser
        column = 0;
        free(cmd);

        // execute command
        int ret;
        if(script != NULL && parsing_status == 0) {
            ret = sequence__execute(script);
        }
        free_script();

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
        signal(SIGINT, signalHandlerInt);
        interactive_shell();
    }
    clean_shell();
    return 0;
}
