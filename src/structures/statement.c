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
