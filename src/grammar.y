%{
    #include <stdio.h>
    extern int yylineno;
    int yylex ();
    int yyerror ();

%}

%token <string> STRING QUOTED_STRING IDENTIFIER
%token IF THEN ELSE FI
%start script
%union {
  char *string;
}
%%

script
: '\n'
| '\n' script       /*repeated new lines */
| sequence
| sequence '\n'
| sequence '\n' script
;

sequence
: sequence_element
| sequence ';' sequence_element
;

sequence_element
: parallel_command
| declaration
;

parallel_command
: piped_command
| parallel_command '&'
| parallel_command '&' ' '      /* useful for the conditions */
| parallel_command '&' pre_spaced_command
| parallel_command '|' pre_spaced_command
;

declaration
: var_name argument     /* old version: IDENTIFIER '=' argument */
;

piped_command
: pre_spaced_command
| piped_command '|' pre_spaced_command
;

pre_spaced_command
: post_spaced_command
| ' ' pre_spaced_command
;

post_spaced_command
: post_spaced_command ' '
| command
;

command
: conditional
| argument_list
;

conditional
: IF sequence THEN sequence FI
| IF sequence ELSE sequence FI
| IF sequence THEN sequence ELSE sequence FI
;

argument_list
: argument
| argument_list ' '
| argument_list ' ' argument
;

argument
: string
| quoted_string
;

string
: STRING                    {printf("STRING:%s\n",$1);}
| var_name
| STRING var_name string
;

quoted_string
: quoted_string_content     /* old version: '"' quoted_string_content '"' */
;

quoted_string_content
: QUOTED_STRING             {printf("QUOTED_STRING:%s\n",$1);}
| QUOTED_STRING var_name quoted_string_content
;

var_name
: IDENTIFIER                {printf("IDENTIFIER:%s\n",$1);}     /* old version: '$' IDENTIFIER */
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
