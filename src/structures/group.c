#include "group.h"



/**
 * Allocate a new group structure and initialize its elements
 */
struct group* group__new(struct sequence * sequence) {
    struct group* group = malloc(sizeof(struct group));
    group->sequence = sequence;
    return group;
}


/**
 * Free a group structure and all its contents
 */
void group__free(struct group * g) {
    sequence__free(g->sequence);
    free(g);
}

/**
 * Execute a group by executing the sequence contained in the group
 * Return the child process ID of the sequence or '1' if an error occured when forking
 */
int group__execute(struct group * g) {
    int cpid = fork();
    if(cpid == 0) {
        int ret = sequence__execute(g->sequence);
        exit(ret);
    } else if(cpid < 0) {
        perror("Can't fork");
        return 1;
    } else {
        return cpid;
    }
}
