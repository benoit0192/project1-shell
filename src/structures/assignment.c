#include "assignment.h"

#define REPL_BUFF_CHUNK 100

// List of all defined environment variables in a linked list
struct environment_variable {
    char *var_name;
    char *var_value;
    struct environment_variable *next;
};

// Declare the structure that will contain the environment variables
struct environment_variable *vars = NULL;

/**
 * Add a variable at the end of the linked list if it does not exist yet
 * If it does, simply overwrite the content of the existing content variable by the new one
 */
void environment_variable__add(char *var_name, char* var_value) {
    struct environment_variable *it;
    for(it = vars; it != NULL && strcmp(it->var_name, var_name) != 0; it = it->next) {}
    if(it == NULL) {
        // var not found. add it
        struct environment_variable *new = mymalloc(sizeof(struct environment_variable));
        new->var_name = var_name;
        new->var_value = var_value;
        new->next = vars;
        vars = new;
    } else {
        // var found. edit value
        free(it->var_value);
        it->var_value = var_value;
        free(var_name);
    }
}

/**
 * Return the value of variable var_name.
 * if the variable does not exist, an empty string is returned
 * result must be freed by the user
 */
char *environment_variable__get(char *var_name) {
    struct environment_variable *it;
    for(it = vars; it != NULL && strcmp(it->var_name, var_name) != 0; it = it->next) {}
    if(it == NULL) {
        // var not found.
        return mystrdup("");
    } else {
        // var found.
        return mystrdup(it->var_value);
    }
}

/**
 * Free an environment variable structure and all its contents
 */
void environment_variable__free() {
    struct environment_variable *it = vars;
    struct environment_variable *next;
    while(it != NULL) {
        next = it->next;
        free(it->var_name);
        free(it->var_value);
        free(it);
        it = next;
    }
}

/**
 * Replace all occurences of $xxxx tokens by their corresponding values
 * the string passed in argument is used as a working buffer by the function and
 * is freed.
 * the value of the string where variables have been replaced is returned and
 * must be freed by the user
 */
char * replace_variables(char * cmd) {
    char * res = mymalloc(REPL_BUFF_CHUNK * sizeof(char));
    int res_size = REPL_BUFF_CHUNK;
    int res_i = 0;
    int cmd_i = 0;
    char cmd_prev = 0;
    while(cmd[cmd_i] != 0) {
        if(cmd[cmd_i] == '$' && cmd_prev != '\\') { // beginning of a var name
            char *var_name = &cmd[cmd_i];
            cmd_i++;
            while(   ('0' <= cmd[cmd_i] && cmd[cmd_i] <= '9')     // 0-9
                  || ('A' <= cmd[cmd_i] && cmd[cmd_i] <= 'Z')     // A-Z
                  || ('a' <= cmd[cmd_i] && cmd[cmd_i] <= 'z')
                  || cmd[cmd_i] == '_' ) { // a-z
                // we use cmd buffer to store var_name
                cmd[cmd_i-1] = cmd[cmd_i];
                cmd_i++;
            }
            cmd[cmd_i-1] = 0; // null-terminate var_name
            char *var_value = environment_variable__get(var_name);
            //copy it in result string
            for(int i = 0; var_value[i] != 0; ++i, ++res_i) {
                if(res_i+1 >= res_size) {
                    res_size += REPL_BUFF_CHUNK;
                    res = myrealloc(res, res_size);
                }
                res[res_i] = var_value[i];
            }
            free(var_value);
            cmd_prev = '$';
        } else {
            if(res_i+1 >= res_size) {
                res_size += REPL_BUFF_CHUNK;
                res = myrealloc(res, res_size);
            }
            res[res_i] = cmd[cmd_i];
            cmd_prev = cmd[cmd_i];
            ++res_i;
            ++cmd_i;
        }
    }
    free(cmd);
    res[res_i] = 0;
    return res;
}

/**
 * Allocate a new assignment structure and set its fields to the values passed in arguments before returning it.
 */
struct assignment* assigment__new(char *varname, char* content) {
    struct assignment* a = mymalloc(sizeof(struct assignment));
    a->varname = varname;
    a->content = content;
    return a;
}

/**
 * Execute an assignment
 * Return 0 if no error occured
 */
int assignment__execute(struct assignment *a) {
    environment_variable__add(mystrdup(a->varname), replace_variables(mystrdup(a->content)));
    return 0;
}


/**
 * Free an assignment structure and all its fields
 */
 void assignment__free(struct assignment *a) {
     free(a->varname);
     free(a->content);
     free(a);
 }
