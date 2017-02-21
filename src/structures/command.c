#include "command.h"

/**
 * allocate a new command structure and initialize its elements
 */
struct command* command__new() {
    struct command* command = malloc(sizeof(struct command));
    command->prog_name = NULL;
    command->args = NULL;
    return command;
}

/**
 * append the argument 'arg' at the end of the argument list of 'cmd'
 * the argument should already have been duplicated with strdup()
 */
void command__append_arg(struct command *cmd, char *arg) {
    struct arg_list *prev    = cmd->args;
    struct arg_list *current = cmd->args;
    while(current != NULL) {
        prev = current;
        current = current->next;
    }
    if(prev == NULL) {
        cmd->args = malloc(sizeof(struct arg_list));
        cmd->args->arg = arg;
        cmd->args->next = NULL;
    } else {
        prev->next = malloc(sizeof(struct arg_list));
        prev->next->arg = arg;
        prev->next->next = NULL;
    }
}

/**
 * Frees a command structure and all its contents
 */
void command__free(struct command *cmd) {
    free(cmd->prog_name);
    struct arg_list *current = cmd->args;
    while(current != NULL) {
        struct arg_list *next = current->next;
        free(current->arg);
        free(current);
        current = next;
    }
    free(cmd);
}

int command__execute(struct command *cmd) {
    // execute builtins
    if(strcmp(cmd->prog_name, "cd") == 0) {
        if(cmd->args == NULL)
            cd(NULL);
        else
            cd(cmd->args->arg);
        return 0;
    }

    int cpid = fork();
    if(cpid > 0) {
        return cpid;
    } else if(cpid < 0) {
        perror("Can't fork: ");
        return cpid;
    }

    char * prog_name = replace_variables(strdup(cmd->prog_name));
    char * path = environment_variable__get("PATH");
    char *path_current = path;
    char *absolute = NULL;
    if(prog_name[0] != '/' && prog_name[0] != '.') {
        char * cwd = getcwd(NULL, 0);
        if(cwd == NULL) {
            perror("Can't get current working directory: ");
            _exit(errno);
        }
        if(asprintf(&absolute, "%s/%s", cwd, prog_name) == -1) {
            fprintf(stderr, "Can't build absolute path\n");
            _exit(1);
        }
        int i = 0;
        int stop = 0;
        while(access(absolute, X_OK) == -1) {
            if(stop) {
                stop = 2;
                break;
            }
            while(path[i] != 0 && path[i] != ':') {
                ++i;
            }
            if(path[i] == 0)
                stop = 1;
            path[i] = 0;
            free(absolute);
            if(asprintf(&absolute, "%s/%s", path_current, prog_name) == -1) {
                fprintf(stderr, "Can't build absolute path\n");
                _exit(1);
            }
            ++i;
            path_current= &path[i];
        }
        if(stop == 2) {
            fprintf(stderr, "%s: command not found\n", prog_name);
            _exit(1);
        }
    } else {
        absolute = prog_name;
    }

    // count number of arguments
    struct arg_list *it = cmd->args;
    int n = 0;
    while(it != NULL) {
        ++n;
        it = it->next;
    }
    char *args[n+2];
    args[0] = absolute;
    it = cmd->args;
    n = 1;
    while(it != NULL) {
        args[n] = replace_variables(strdup(it->arg));
        ++n;
        it = it->next;
    }
    args[n] = NULL;
    execv(absolute, args);
    perror("Can't run execvp: ");
    _exit(1);
}
