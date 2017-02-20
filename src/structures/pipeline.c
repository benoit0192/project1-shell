#include "pipeline.h"


/**
 * append the statement 'st' at the end of the pipeline list 'pipe'
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
        prev = malloc(sizeof(struct pipeline));
        prev->statement = st;
        prev->next = NULL;
        return prev;
    } else {
        prev->next = malloc(sizeof(struct pipeline));
        prev->next->statement = st;
        prev->next->next = NULL;
        return pipe;
    }
}


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

int pipeline__execute(struct pipeline *pipeline) {
    // save default input and output files
    int in = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);

    // set the initial input
    int fdin = dup(in);
    int fdout;

    int cpid;
    for(struct pipeline *current = pipeline; current != NULL; current = current->next) {
        // redirect input
        dup2(fdin, STDIN_FILENO);
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

        // check builtins 

        cpid = fork();
        if(cpid == 0) {
            // child
            _exit(statement__execute(current->statement));
        }
    }

    // restore default input and output files
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);

    if(!pipeline->background) {
        // wait for last command
        int status;
        waitpid(cpid, &status, 0);
        if(WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return 1;
    }
    return 0;
}
