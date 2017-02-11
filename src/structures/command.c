#include "command.h"

/**
 * allocate a new command structure and initialize its elements
 */
struct command* command__new() {
    struct command* command = malloc(sizeof(struct command));
    command->prog_name = NULL;
    command->args = NULL;
    command->background = 0;
    command->fd_in = 0;
    command->fd_out = 1;
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
    prev->next = malloc(sizeof(struct arg_list));
    prev->next->arg = arg;
    prev->next->next = NULL;
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
}
