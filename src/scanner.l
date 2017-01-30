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
"#".*					{ count(); printf("comment\n");}
"\""                  	{ count(); printf("quote\n"); return('\"'); }
" "*				  	{ count(); printf("space\n"); return(' '); }		/* multiple consecutive spaces only counted as one */
"\n"			      	{ count(); printf("new_line\n"); return('\n'); }
"|"				      	{ count(); printf("pipe\n"); return('|'); }
"&"				      	{ count(); printf("ampersand\n"); return('&'); }
"="				      	{ count(); printf("equal\n"); return('='); }
";"				      	{ count(); return(';'); }
"$"				      	{ count(); return('$'); }
"if"			      	{ count(); return(IF); }
"then"			      	{ count(); return(THEN); }
"else"			      	{ count(); return(ELSE); }
"fi"			      	{ count(); return(FI); }
(\\.|[^"# \n|&=;\$])* 	{ count(); yylval.string=strdup(yytext); printf("match:%s\n",yylval.string); return(STRING); }
\"(\\.|[^\"])*\"     	{ 	count(); 
							yylval.string = calloc(strlen(yytext)-1, sizeof(char));			/* remove quotes */
							strncpy(yylval.string, &yytext[1], strlen(yytext)-2);
							printf("quote_match:%s\n",yylval.string); 
							return(QUOTED_STRING);
					  	}
[$]?(\\.|[^ =\n])*[=]?	{ 	count(); 
							yylval.string=strdup(yytext);
							if((char)yytext[strlen(yytext)-1] == '='){						/* remove '=' */
								yylval.string = calloc(strlen(yytext), sizeof(char));
								strncpy(yylval.string, &yytext[0], strlen(yytext)-1);
								printf("var_match:%s\n",yylval.string); 
							}
							else{															/* remove '$' */
								yylval.string = calloc(strlen(yytext), sizeof(char));
								strncpy(yylval.string, &yytext[1], strlen(yytext));
								printf("var_match:%s\n",yylval.string); 
							}
							return(IDENTIFIER); 
						}

%%
// (\\.|[^"# \n|&=;\$])* 	{ count(); yylval.string=strdup(yytext); return(STRING); }
// (\\.|[^"\$])*         	{ count(); yylval.string=strdup(yytext); return(QUOTED_STRING); }
// [$]?{L}({L}|{D})*	  	{ count(); yylval.string=strdup(yytext); return(IDENTIFIER); }
// .	 				  	{ /* ignore bad characters */ }
// [\t\v\f]			  		{ count(); }

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
