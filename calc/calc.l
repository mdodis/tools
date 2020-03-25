%{
    #include <stdlib.h>
	int yyerror(char*);

%}

%%

[0-9]+   	{yylval = atoi(yytext); return 1;}
\n		    {return 0;}
"+"			{return '+';}
"-"			{return '-';}
"*"			{return '*';}
"/"			{return '/';}
.			;
%%

int yywrap(void) {
    return 1;
}

int main() {
	yylex();
}
