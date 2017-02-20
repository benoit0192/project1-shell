#include "cd.h"


//getcwd(NULL, 0);

int cd(char * path) {
    int ret;
    if(path == NULL) {
        // no argument was passed to cd, so go $HOME
        char * home = environment_variable__get("HOME");
        ret = cd(home);
        free(home);
        return ret;
    }
    ret = chdir(path);
    if(ret)
        fprintf(stderr, "Can't cd to %s\n", path);
    return ret;
}
