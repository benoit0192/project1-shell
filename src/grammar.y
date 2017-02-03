%{
    #include <stdio.h>
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
#include <stdio.h>
#include <string.h>

extern char yytext[];
extern int column;
extern int yylineno;
extern FILE *yyin;

char *file_name = NULL;

int yyerror (char *s) {
    fflush (stdout);
    fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
    return 0;
}


int main (int argc, char *argv[]) {
    FILE *input = NULL;
    if (argc==2) {
        input = fopen (argv[1], "r");
        file_name = strdup (argv[1]);
	    if (input) {
	        yyin = input;
	    } else {
	        fprintf (stderr, "%s: Could not open %s\n", *argv, argv[1]);
	        return 1;
	    }
    } else {
	    fprintf (stderr, "%s: error: no input file\n", *argv);
	    return 1;
    }
    yyparse ();
    free (file_name);
    return 0;
}
