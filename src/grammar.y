%{
    #include <stdio.h>
    #include "structures/command.h"
    extern int yylineno;
    int yylex ();
    int yyerror ();

%}

%token <string> WORD ASSIGNMENT_WORD
%start script
%union {
  char *string;
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
: ASSIGNMENT_WORD WORD
| ASSIGNMENT_WORD
;

command
:         WORD
| command WORD
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
: '&'
| ';'
;

%%
// name of the script file being parsed (if reading from file)
// initialized in main()
extern char *file_name;

// position in the file, set by the scanner
extern int column;
extern int yylineno;


int yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
    return 0;
}
