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
