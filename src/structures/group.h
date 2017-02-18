#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdlib.h>
#include "sequence.h"

struct group {
    struct sequence * sequence;
    int background;
    int fd_in;
    int fd_out;
};

/**
 * allocate a new command structure and initialize its elements
 */
struct group* group__new(struct sequence * sequence);


/**
 * Frees a group structure and all its contents
 */
void group__free(struct group * g);

#endif // _GROUP_H_