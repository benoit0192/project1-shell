#ifndef _GROUP_H_
#define _GROUP_H_

#include <stdlib.h>
#include "sequence.h"

#include "../errors.h"

/*---- Group ---------------------------------------------------------
*   This structure stores the structure of a sequence contained inside
*   the group.
*--------------------------------------------------------------------------*/

struct group {
    struct sequence * sequence;
};

/**
 * Allocate a new command structure and initialize its elements
 */
struct group* group__new(struct sequence * sequence);


/**
 * Free a group structure and all its contents
 */
void group__free(struct group * g);

/**
 * Execute a group by executing the sequence contained in the group
 * Return the child process ID of the sequence or '1' if an error occured when forking
 */
int group__execute(struct group * g);

#endif // _GROUP_H_
