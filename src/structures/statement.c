#include "statement.h"


/**
 * Free a statement structure and all its fields depending on the type of the statement (command, assignement or group)
 */
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

/**
 * Execute a statement by calling the execution function of its 'statement_type' attribute that
 * match the type of the attribute (command, assignment or group) and return the return value of the execution function
 * If no match has been found, return -1
 */
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
