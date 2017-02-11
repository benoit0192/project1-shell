#include "assignment.h"


/**
 * Allocates a new assignment structure and sets its fields
 */
struct assignment* assigment__new(char *varname, char* content) {
    struct assignment* a = malloc(sizeof(struct assignment));
    a->varname = varname;
    a->content = content;
    return a;
}

/**
 * Frees an assignment structure and all its fields
 */
 void assignment__free(struct assignment *a) {
     free(a->varname);
     free(a->content);
     free(a);
 }
