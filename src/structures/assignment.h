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

 /**
  * Execute assignment
  * returns 0 if no error occured
  */
 int assignment__execute(struct assignment *a);

 /**
  * replace all occurences of $xxxx tokens by their corresponding values
  * the string passed in argument is used as a working buffer by the function and
  * is freed.
  * the value of the string where variables have been replaced is returned and
  * must be freed by the user
  */
 char * replace_variables(char * cmd);

char * environment_variable__get(char *var_name);

void environment_variable__free();

#endif // _ASSIGNMENT_H_
