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
 * Allocate a new assignment structure and sets its fields
 */
struct assignment* assigment__new(char *varname, char* content);

/**
 * Free an assignment structure and all its fields
 */
void assignment__free(struct assignment *a);

 /**
  * Execute an assignment
  * returns 0 if no error occured
  */
int assignment__execute(struct assignment *a);

 /**
  * Replace all occurences of $xxxx tokens by their corresponding values
  * the string passed in argument is used as a working buffer by the function and
  * is freed.
  * the value of the string where variables have been replaced is returned and
  * must be freed by the user
  */
char * replace_variables(char * cmd);

/**
 * Return the value of variable var_name.
 * if the variable does not exist, an empty string is returned
 * result must be freed by the user
 */
char * environment_variable__get(char *var_name);

/**
 * Free an environment variable structure and all its contents
 */
void environment_variable__free();

#endif // _ASSIGNMENT_H_
