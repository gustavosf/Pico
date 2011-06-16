 /* Secao das declaracoes */

%option noyywrap
%{
    #include <stdlib.h>
    #include <string.h>
   
    #include "tokens.h"

	int    VAL_INT = 0;
	double VAL_DOUBLE = 0;
  
%}
%x IN_COMMENT 

 /* Definicoes regulares */

digito    [0-9]
maiuscula [A-Z]
minuscula [a-z]
letra     {maiuscula}|{minuscula}

%%

 /* Secao das Regras de traducao */

 /* lexemas de tipo */
int    { return(INT); }
double { return(DOUBLE); }
real   { return(REAL); }
char   { return(CHAR); }

 /* lexemas gerais */
"*"    { return('*'); }
"+"    { return('+'); }
"-"    { return('-'); }
"/"    { return('/'); }
","    { return(','); }
";"    { return(';'); }
":"    { return(':'); }
"("    { return('('); }
")"    { return(')'); }
"["    { return('['); }
"]"    { return(']'); }
"{"    { return('{'); }
"}"    { return('}'); }
"<"    { return('<'); }
">"    { return('>'); }
":="   { return('='); }

"'"    { return(QUOTE); }
"\""   { return(DQUOTE); }

 /* lexemas de operacoes */
"<="   { return(LE); }
">="   { return(GE); }
"="    { return(EQ); }
"!="   { return(NE); }
"&&"   { return(AND); }
"||"   { return(OR); }
"!"    { return(NOT); }

 /* lexemas de controle */
if     { return(IF); }
then   { return(THEN); }
else   { return(ELSE); }
for    { return(FOR); }
next   { return(NEXT); }
while  { return(WHILE); }
repeat { return(REPEAT); }
until  { return(UNTIL); }
end    { return(END); }
case   { return(CASE); }

 /* lexemas booleanos */
true   { return(TRUE); }
false  { return(FALSE); }

 /* reconhecimento de caracteres 'brancos' */
[ \r\n\t]+ { printf("SPACE\n"); }

 /* lexemas complexos */
({minuscula}|_)({letra}|{digito}|_|#)*    { return(IDF); }
{maiuscula}({maiuscula}|{digito}|_)*      { return(CONST); }
{digito}+                                 { 
	yylval.cadeia = (char*) malloc(strlen(yytext)+1);
    strcpy(yylval.cadeia, yytext);
    return(INT_LIT); 
}
{digito}+\.{digito}+([eE][+-]?{digito}+)? { 
	yylval.cadeia = (char*) malloc(strlen(yytext)+1);
    strcpy(yylval.cadeia, yytext);
	return(F_LIT); 
}

 /* reconhecimento de comentarios
  * http://flex.sourceforge.net/manual/How-can-I-match-C_002dstyle-comments_003f.html
  */
<INITIAL>{
  "/*"      BEGIN(IN_COMMENT); printf("#COMM#");
}
<IN_COMMENT>{
  "*/"      BEGIN(INITIAL); printf("\n");
  [^*\n]+   printf("%s", yytext); // eat comment in chunks
  "*"       // eat the lone star
  \n        yylineno++; 
}

 /* Tratamento dos erros lexicais */
. { printf("Erro lexical - caractere nao reconhecido: %c.\n", yytext[0]); exit(-1); }

%%

extern FILE *yyin;
