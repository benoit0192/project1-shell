%{
    #include <stdio.h>
    #include "structures/command.h"
    #include "structures/assignment.h"
    extern int yylineno;
    int yylex ();
    int yyerror ();

%}

%token <string> WORD ASSIGNMENT_WORD
%type <string> separator
%type <command> command
%type <assignment> assignment
%type <string> '&' ';'
%start script
%union {
  char *string;
  struct command *command;
  struct assignment *assignment;
}
%%

script
: line_break command_lines line_break
| line_break
;

command_lines
: command_lines newline_list command_line
|                            command_line
;

command_line
: sequence separator
| sequence
;

sequence
:                    pipeline
| sequence separator pipeline
;

pipeline
:              statement
| pipeline '|' statement
;

statement
: assignment
| command
| group
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
: '(' sequence ')'
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
: '&' { $$ = $1; }
| ';' { $$ = $1; }
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
