%{
    #include <stdio.h>
    extern int yylineno;
    int yylex ();
    int yyerror ();

%}

%token <string> IDENTIFIER CONSTANTI
%token IF THEN ELSE
%token RRIGHT
%token ANYTHING
%start program
%union {
  char *string;
}
%%

program
: statement_list
;

statement_list
: statement
| statement_list statement
;

statement
: compound_statement
| definition_statement
| selection_statement
| parallel_statement_list
| pipe_statement_list
;

compound_statement
: '(' statement_list ')'
;

definition_statement
: IDENTIFIER '=' IDENTIFIER
;

selection_statement
: IF statement THEN statement
| IF statement THEN statement ELSE statement
;

parallel_statement_list
: statement '&' statement
| parallel_statement_list '&' statement
;

pipe_statement_list
: statement '|' statement
| pipe_statement_list '|' statement

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
	}
	else {
	  fprintf (stderr, "%s: Could not open %s\n", *argv, argv[1]);
	    return 1;
	}
    }
    else {
	fprintf (stderr, "%s: error: no input file\n", *argv);
	return 1;
    }
    yyparse ();
    free (file_name);
    return 0;
}
