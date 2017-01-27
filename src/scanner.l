%{
#include <stdio.h>
#include <string.h>
#include "grammar.tab.h"
void count();
%}
D[0-9]
L[a-zA-Z_]
%option yylineno
%%
"#".*			      { count(); }
"\""                  { count(); return('\"'); }
" "				      { count(); return(' '); }
"\n"			      { count(); return('\n'); }
"|"				      { count(); return('|'); }
"&"				      { count(); return('&'); }
"="				      { count(); return('='); }
";"				      { count(); return(';'); }
"$"				      { count(); return('$'); }
"if"			      { count(); return(IF); }
"then"			      { count(); return(THEN); }
"else"			      { count(); return(ELSE); }
"fi"			      { count(); return(FI); }
(\\.|[^"# \n|&=;\$])* { count(); yylval.string=strdup(yytext); return(STRING); }
(\\.|[^"\$])*         { count(); yylval.string=strdup(yytext); return(QUOTED_STRING); }
[$]?{L}({L}|{D})*	  { count(); yylval.string=strdup(yytext); return(IDENTIFIER); }

%%
//.	 				  { /* ignore bad characters */ }
//[\t\v\f]			  { count(); }
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
