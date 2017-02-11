#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include <stdlib.h>
#include <string.h>

/*---- Assignment ---------------------------------------------------------
*   This structure stores the variable name of an assignment statement
*   and the content to be assigned.
*--------------------------------------------------------------------------*/

struct assignment {
    char *varname;
    char *content;
};

/**
 * Allocates a new assignment structure and sets its fields
 */
struct assignment* assigment__new(char *varname, char* content);

/**
 * Frees an assignment structure and all its fields
 */
 void assignment__free(struct assignment *a);


#endif // _ASSIGNMENT_H_
