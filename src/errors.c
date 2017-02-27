#include "errors.h"


/**
 * like stdlib.h's malloc but exits with error if malloc was unsuccessful
 */
void * mymalloc(size_t s) {
    void * r = malloc(s);
    if(r == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(1);
    }
    return r;
}

/**
 * like stdlib.h's realloc but exits with error if realloc was unsuccessful
 */
void * myrealloc(void * mem, size_t s) {
    void * r = realloc(mem, s);
    if(r == NULL) {
        fprintf(stderr, "Could not re-allocate memory\n");
        exit(1);
    }
    return r;
}

/**
 * like string.h's strdup but exits with error if strdup was unsuccessful
 */
char * mystrdup(char * s) {
    char * cpy = strdup(s);
    if(cpy == NULL) {
        fprintf(stderr, "Could not duplicate string\n");
        exit(1);
    }
    return cpy;
}
