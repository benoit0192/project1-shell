%{
    #include <stdio.h>
    #include "structures/sequence.h"
    #include "structures/statement.h"
    #include "structures/command.h"
    #include "structures/assignment.h"
    #include "structures/group.h"
    extern int yylineno;
    int yylex ();
    int yyerror ();

%}

%token <string> WORD ASSIGNMENT_WORD
%type <num> separator
%type <sequence> sequence command_line command_lines script
%type <statement> statement pipeline  // TODO: implement pipeline
%type <command> command
%type <assignment> assignment
%type <group> group
%start script
%union {
  char *string;
  int num;
  struct statement *statement;
  struct sequence *sequence;
  struct command *command;
  struct assignment *assignment;
  struct group *group;
}
%%

script
: line_break command_lines line_break { $$ = $2; }
| line_break { $$ = NULL; }
;

command_lines
: command_lines newline_list command_line { $$ = $1; }
|                            command_line { $$ = $1; }
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
:              statement { $$ = $1; }
| pipeline '|' statement { $$ = $1; } // TODO pipeline
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
: '(' sequence ')' {
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
    fprintf (stderr, "%s:%d:%d: %s\n", script_filename, yylineno, column, s);
    return 0;
}
