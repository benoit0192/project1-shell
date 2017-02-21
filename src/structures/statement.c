#include "statement.h"



void statement__free(struct statement *st) {
    switch(st->type) {
        case STATEMENT_TYPE_COMMAND:
            command__free(st->statement.command);
            break;
        case STATEMENT_TYPE_ASSIGNMENT:
            assignment__free(st->statement.assignment);
            break;
        case STATEMENT_TYPE_GROUP:
            group__free(st->statement.group);
            break;
        default:
            break;
    }
    free(st);
}


int statement__execute(struct statement *st) {
    switch(st->type) {
        case STATEMENT_TYPE_COMMAND:
            return command__execute(st->statement.command);
            break;
        case STATEMENT_TYPE_ASSIGNMENT:
            return assignment__execute(st->statement.assignment);
            break;
        case STATEMENT_TYPE_GROUP:
            return group__execute(st->statement.group);
            break;
        default:
            return -1;
            break;
    }

}
