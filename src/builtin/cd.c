#include "cd.h"


//getcwd(NULL, 0);

int cd(struct arg_list *args) {
    int ret;
    if(args == NULL) {
        // no argument was passed to cd, so go $HOME
        char * home = environment_variable__get("HOME");
        if(home[0] == 0) {
            fprintf(stderr, "Can't cd to $HOME: $HOME is not set");
            ret = 0;
        } else
            ret = chdir(home);
        if(ret)
            fprintf(stderr, "Can't cd to \"%s\": %s\n", home, strerror(errno));
        free(home);
    } else {
        ret = chdir(args->arg);
        if(ret)
            fprintf(stderr, "Can't cd to \"%s\": %s\n", args->arg, strerror(errno));
    }
    return ret;
}
