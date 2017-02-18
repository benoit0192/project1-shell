#include "assignment.h"

#define REPL_BUFF_CHUNK 100

// the list of all defined variables
struct environment_variable {
    char *var_name;
    char *var_value;
    struct environment_variable *next;
};

struct environment_variable *vars = NULL;

void environment_variable__add(char *var_name, char* var_value) {
    struct environment_variable *it;
    for(it = vars; it != NULL && strcmp(it->var_name, var_name) != 0; it = it->next) {}
    if(it == NULL) {
        // var not found. add it
        struct environment_variable *new = malloc(sizeof(struct environment_variable));
        new->var_name = var_name;
        new->var_value = var_value;
        new->next = vars;
        vars = new;
    } else {
        // var found. edit value
        it->var_value = var_value;
    }
}

/**
 * returns the value of variable var_name.
 * if the variable does not exist, an empty string is returned
 * result must be freed by the user
 */
char *environment_variable__get(char *var_name) {
    struct environment_variable *it;
    for(it = vars; it != NULL && strcmp(it->var_name, var_name) != 0; it = it->next) {}
    if(it == NULL) {
        // var not found.
        return strdup("");
    } else {
        // var found.
        return strdup(it->var_value);
    }
}

/**
 * replace all occurences of $xxxx tokens by their corresponding values
 * the string passed in argument is used as a working buffer by the function and
 * is freed.
 * the value of the string where variables have been replaced is returned and
 * must be freed by the user
 */
char * replace_variables(char * cmd) {
    char * res = malloc(REPL_BUFF_CHUNK * sizeof(char));
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
                  || ('a' <= cmd[cmd_i] && cmd[cmd_i] <= 'z') ) { // a-z
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
                    res = realloc(res, res_size);
                }
                res[res_i] = var_value[i];
            }
            free(var_value);
            cmd_prev = '$';
        } else {
            if(res_i+1 >= res_size) {
                res_size += REPL_BUFF_CHUNK;
                res = realloc(res, res_size);
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
 * Allocates a new assignment structure and sets its fields
 */
struct assignment* assigment__new(char *varname, char* content) {
    struct assignment* a = malloc(sizeof(struct assignment));
    a->varname = varname;
    a->content = content;
    return a;
}

/**
 * Execute assignment
 * returns 0 if no error occured
 */
int assignment__execute(struct assignment *a) {
    environment_variable__add(strdup(a->varname), replace_variables(strdup(a->content)));
    return 0;
}


/**
 * Frees an assignment structure and all its fields
 */
 void assignment__free(struct assignment *a) {
     free(a->varname);
     free(a->content);
     free(a);
 }
