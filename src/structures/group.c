#include "group.h"



/**
 * allocate a new group structure and initialize its elements
 */
struct group* group__new() {
    struct group* group = malloc(sizeof(struct group));
    group->sequence = NULL;
    group->background = 0;
    group->fd_in = 0;
    group->fd_out = 1;
    return group;
}


/**
 * Frees a group structure and all its contents
 */
void group__free(struct group * g) {
    sequence__free(g->sequence);
    free(g);
}
