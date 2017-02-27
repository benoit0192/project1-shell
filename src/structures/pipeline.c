#include "pipeline.h"


/**
 * Append the statement 'st' at the end of the pipeline list 'pipe'
 * 'pipe' might be NULL for the empty list.
 * This function returns the new first element of the pipeline. unchanged if the list was not empty (NULL).
 */
struct pipeline * pipeline__append_statement(struct pipeline *pipe, struct statement *st) {
    struct pipeline *prev    = pipe;
    struct pipeline *current = pipe;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        prev = mymalloc(sizeof(struct pipeline));
        prev->statement = st;
        prev->background = 0;
        prev->next = NULL;
        return prev;
    } else {
        prev->next = mymalloc(sizeof(struct pipeline));
        prev->next->statement = st;
        prev->background = 0;
        prev->next->next = NULL;
        return pipe;
    }
}

/**
 * Free a pipeline structure and all its contents
 */
void pipeline__free(struct pipeline *p) {
    struct pipeline *current = p;
    struct pipeline *next = p;
    while(current != NULL) {
        next = current->next;
        statement__free(current->statement);
        free(current);
        current = next;
    }
}

/**
 * Execute a pipeline by executing each statement piped into another one
 * If a pipeline is not executed in background, set the alarm for 5s if enabled then return the status of the last pipeline statement.
 * Otherwise return 0.
 */
int pipeline__execute(struct pipeline *pipeline) {
    // save default input and output files
    int in = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);

    // set the initial input
    int fdin = dup(in);

    int cpid;
    for(struct pipeline *current = pipeline; current != NULL; current = current->next) {
        // redirect input
        dup2(fdin, STDIN_FILENO);
        int fdout;
        close(fdin);

        // redirect output
        if(current->next == NULL){
            // last command of the pipeline, restore output file
            fdout = dup(out);
        } else {
            // not last command
            // create pipe
            int fdpipe[2];
            pipe(fdpipe);
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }

        // Redirect output
        dup2(fdout, STDOUT_FILENO);
        close(fdout);

        if( (cpid = statement__execute(current->statement)) < 0) {
            // error
            fprintf(stderr, "Can't execute command\n");
            return 1;
        }
    }

    // restore default input and output files
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);

    if(!pipeline->background && cpid > 0) {
        // wait for last command
        char *alarm_str = environment_variable__get("ALARM");
        int disable_alarm = strcmp(alarm_str, "true");
        free(alarm_str);
        if(!disable_alarm)
            alarm(5);
        int status;
        waitpid(cpid, &status, 0);
        alarm(0);
        if(WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return 1;
        }
    }
    return 0;
}
