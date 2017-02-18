#include "autocomplete.h"

// maximum number of commands kept in memory
#define HISTORY_SIZE 100

struct history_elt {
    char * value;
    int size;
    struct history_elt * prev;
    struct history_elt * next;
};

struct history {
    struct history_elt * first;
    struct history_elt * last;
    int size;
    struct history_elt * current;
};

int autocomplete_size = 0;
char * autocomplete_start = NULL;
int cycle_way = 0;
struct history history;


void reset_autocomplete() {
    if(autocomplete_start != NULL) {
        free(autocomplete_start);
    }
    autocomplete_start = NULL;
    autocomplete_size = 0;
    history.current = NULL;
}



/**
 * pops last element from history and reset current suggestion and autocomplete parameter
 * The user must free the returned value himself
 */
char * history_pop() {
    char * ret = NULL;
    if(history.size > 0) {
        struct history_elt * tmp = history.last;
        if(history.last == history.first) { // only one element in list
            history.first = NULL;
            history.last = NULL;
        } else { // at least two elements
            history.last = tmp->prev;
            history.last->next = NULL;
        }
        ret = tmp->value;
        free(tmp);
        history.size--;
        reset_autocomplete();
    }
    return ret;
}

int line_is_empty(char * line) {
    int i;
    for(i = 0; line[i] == ' '; ++i) {}
    return line[i] == 0;
}

/**
 * Adds new element to history and reset current suggestion and autocomplete parameter
 */
void history_push(char * value) {
    // pop last elements if size is exceeding HISTORY_SIZE
    while(history.size >= HISTORY_SIZE) {
        free(history_pop());
    }
    if(!line_is_empty(value)){
        // push new one
        struct history_elt * tmp = malloc(sizeof(struct history_elt));
        tmp->value = strdup(value);
        tmp->size = strlen(value);
        tmp->prev = NULL;
        tmp->next = history.first;
        if(history.first != NULL)
            history.first->prev = tmp;
        history.first = tmp;
        if(history.last == NULL)
            history.last = tmp;
        history.size++;
        reset_autocomplete();
    }
}

void history_load(char * filename) {
    // initialize an empty history
    history.first   = NULL;
    history.last    = NULL;
    history.size    = 0;
    history.current = NULL;

    // load file contents
    char *line = NULL;
    size_t size = 0;
    FILE *f = fopen(filename, "r");
    if(f == NULL) {
        fprintf(stderr, "\033[33mWARNING\033[39m No history file found (%s). A new history file will be created\n", filename);
        return;
    }
    int n;
    while (getline(&line, &size, f) != -1) {
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = 0; // remove trailing carriage returns if any
        history_push(line);
        line = NULL;
        size = 0;
    }
    free(line); // free line even if getline returns an error (EOF)
    fclose(f);
}

void history_save(char * filename) {
    // save history to file
    FILE *f = fopen(filename, "w");
    char * line;
    while( (line = history_pop()) != NULL ) {
        if(f != NULL) fprintf(f, "%s\n", line);
        free(line);
    }
    if(f == NULL)
        fprintf(stderr, "\033[31mERROR\033[39m Could not save history file (%s).\n", filename);
    else
        fclose(f);
}

char * update_current(struct history_elt * it) {
    if(it == NULL) {
        if(history.current != NULL)
            return strdup(history.current->value);
        else
            return NULL;
    } else {
        history.current = it;
        return strdup(history.current->value);
    }
}

char * history_prev(struct input_buffer * inbuff) {
    struct history_elt * it;
    if(autocomplete_start == NULL) {
        return NULL;
    } else {
        if(inbuff->pos != autocomplete_size
           || strncmp(autocomplete_start, inbuff->buff, inbuff->pos) != 0) {
            reset_autocomplete();
            return NULL;
        } else {
            if(history.current == NULL) return NULL;
            it = history.current->prev;
        }
    }
    while(it != NULL && strncmp(autocomplete_start, it->value,
                                (autocomplete_size < it->size) ? autocomplete_size : it->size) != 0) {
        it = it->prev;
    }

    if(it == NULL) {
        reset_autocomplete();
        history.current = NULL;
        return NULL;
    } else {
        history.current = it;
        return strdup(history.current->value);
    }
}

char * history_next(struct input_buffer * inbuff) {
    struct history_elt * it;
    if(autocomplete_start == NULL) {
        autocomplete_start = strdup(inbuff->buff);
        autocomplete_start[inbuff->pos] = 0;
        autocomplete_size  = inbuff->pos;
        it = history.first;
    } else {
        if(inbuff->pos != autocomplete_size
           || strncmp(autocomplete_start, inbuff->buff, inbuff->pos) != 0) {
            reset_autocomplete();
            autocomplete_start = strdup(inbuff->buff);
            autocomplete_start[inbuff->pos] = 0;
            autocomplete_size  = inbuff->pos;
            it = history.first;
        } else {
            if(history.current == NULL) return NULL;
            it = history.current->next;
        }
    }
    while(it != NULL && strncmp(autocomplete_start, it->value,
                                (autocomplete_size < it->size) ? autocomplete_size : it->size) != 0) {
        it = it->next;
    }

    return update_current(it);
}

/**
 * called when user press tab. cycles through completions
 */
char * history_match(struct input_buffer * inbuff) {
    reset_autocomplete();
    autocomplete_start = strdup(inbuff->buff);
    autocomplete_start[inbuff->pos] = 0;
    autocomplete_size  = inbuff->pos;

    struct history_elt * it;
    for(it = history.first;
        it != NULL && strncmp(autocomplete_start,
                              it->value,
                              (autocomplete_size < it->size) ? autocomplete_size : it->size) != 0;
        it = it->next) {}

    return update_current(it);
}
