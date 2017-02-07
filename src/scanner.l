%{
#include <stdio.h>
#include <string.h>
#include "grammar.tab.h"
void count();
%}
D[0-9]
L[a-zA-Z_]
%option nounput noinput
%option yylineno
%%
"#".*					{ count(); printf("comment\n");}
" "*				  	{ count(); }
"\n"			      	{ count(); printf("new_line\n"); return('\n'); }
"|"				      	{ count(); printf("pipe\n"); return('|'); }
"&"				      	{ count(); printf("ampersand\n"); return('&'); }
"="				      	{ count(); printf("equal\n"); return('='); }
";"				      	{ count(); return(';'); }
"("				      	{ count(); return('('); }
")"				      	{ count(); return(')'); }
(\\.|[^"# \n\|&=;\(\)])+	{ count(); yylval.string = strdup(yytext); printf("match:%s\n",yylval.string); /* replace env vars */ return(WORD); }
\"(\\.|[^\"])*\"     	{ count(); yylval.string = strdup(yytext+1); yylval.string[strlen(yytext)-2] = 0; printf("quote_match:%s\n",yylval.string); /* replace env vars */ return(WORD); }
{L}({L}|{D})*=  	  	{ count(); yylval.string = strdup(yytext); yylval.string[strlen(yytext)-1] = 0; printf("assignment:%s\n",yylval.string); return(ASSIGNMENT_WORD); }
.	 				  	{ /* ignore bad characters */ }

%%

int yywrap() {
	return 1;
}

int column = 0;
/* This function counts the number of character, for debugging purposes */
void count() {
    int i;
	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 4 - (column % 4);
		else
			column++;
	}
}
