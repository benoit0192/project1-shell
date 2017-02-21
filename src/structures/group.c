#include "group.h"



/**
 * allocate a new group structure and initialize its elements
 */
struct group* group__new(struct sequence * sequence) {
    struct group* group = malloc(sizeof(struct group));
    group->sequence = sequence;
    return group;
}


/**
 * Frees a group structure and all its contents
 */
void group__free(struct group * g) {
    sequence__free(g->sequence);
    free(g);
}


int group__execute(struct group * g) {
    int cpid = fork();
    if(cpid == 0) {
        return sequence__execute(g->sequence);
    } else if(cpid < 0) {
        perror("Can't fork");
        return 1;
    } else {
        return cpid;
    }
}
