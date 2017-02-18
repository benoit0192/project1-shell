%{
    #include <stdio.h>
    #include "structures/sequence.h"
    #include "structures/pipeline.h"
    #include "structures/statement.h"
    #include "structures/command.h"
    #include "structures/assignment.h"
    #include "structures/group.h"
    extern int yylineno;
    extern char *yytext;
    int yylex ();
    int yyerror ();
    extern struct sequence *script;
    extern int parsing_status;
%}

%token <string> WORD ASSIGNMENT_WORD
%type <num> separator
%type <sequence> sequence command_line command_lines script
%type <statement> statement
%type <pipeline> pipeline
%type <command> command
%type <assignment> assignment
%type <group> group
%destructor { free($$); } <string>
%destructor { sequence__free($$); } <sequence>
%destructor { statement__free($$); } <statement>
%destructor { pipeline__free($$); } <pipeline>
%destructor { command__free($$); } <command>
%destructor { assignment__free($$); } <assignment>
%destructor { group__free($$); } <group>
%start script
%union {
  char *string;
  int num;
  struct statement *statement;
  struct pipeline *pipeline;
  struct sequence *sequence;
  struct command *command;
  struct assignment *assignment;
  struct group *group;
}
%%

script
: line_break command_lines line_break { $$ = NULL; script = $2; parsing_status = 0; }
| line_break { $$ = NULL; script = NULL; parsing_status = 0; }
;

command_lines
: command_lines newline_list command_line {
    $$ = $1;
    sequence__append_sequence($$, $3);
}
|                            command_line {
    $$ = $1;
}
;

command_line
: sequence separator {
    $$ = $1;
    sequence__set_background($$, $2);
}
| sequence {
    $$ = $1;
}
;

sequence
:                    pipeline {
    $$ = sequence__append_pipeline(NULL, $1);
}
| sequence separator pipeline {
    sequence__set_background($1, $2);
    $$ = sequence__append_pipeline($1, $3);

}
;

pipeline
:              statement {
    $$ = pipeline__append_statement(NULL, $1);
}
| pipeline '|' statement {
    $$ = pipeline__append_statement($1, $3);
}
;

statement
: assignment {
    $$ = malloc(sizeof(struct statement));
    $$->type = STATEMENT_TYPE_ASSIGNMENT;
    $$->statement.assignment = $1;
}
| command {
    $$ = malloc(sizeof(struct statement));
    $$->type = STATEMENT_TYPE_COMMAND;
    $$->statement.command = $1;
}
| group {
    $$ = malloc(sizeof(struct statement));
    $$->type = STATEMENT_TYPE_GROUP;
    $$->statement.group = $1;
}
;

assignment
: ASSIGNMENT_WORD WORD {
    $$ = assigment__new($1, $2);
}
| ASSIGNMENT_WORD {
    $$ = assigment__new($1, strdup(""));
}
;

command
:         WORD {
    $$ = command__new();
    $$->prog_name = $1;
}
| command WORD {
    $$ = $1;
    command__append_arg($$, $2);
}
;

group
: '(' command_line ')' {
    $$ = group__new($2);
}
;

line_break
: newline_list
| /* this can be empty */
;

newline_list
:              '\n'
| newline_list '\n'
;

separator
: '&' { $$ = 1; }
| ';' { $$ = 0; }
;

%%
// name of the script file being parsed (if reading from file)
// initialized in main()
extern char *script_filename;

// position in the file, set by the scanner
extern int column;
extern int yylineno;


int yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s near unexpected token \"%s\"\n", script_filename, yylineno, column, s, yytext);
    parsing_status = 1;
    return 0;
}
