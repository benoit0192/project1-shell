#include "exit.h"

extern void clean_shell();

void builtin_exit(struct arg_list *args) {
    int exit_code;
    if(args == NULL)
        exit_code = 0;
    else
        exit_code = atoi(args->arg);

    clean_shell();

    exit(exit_code);
}
