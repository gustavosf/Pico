 /* Secao das declaracoes */

%option noyywrap
%{
    #include <stdlib.h>
    #include <string.h>
   
    /* este include eh importante... */
    #include "tokens.h"
  
%}
%x IN_COMMENT 

 /* Definicoes regulares (A COMPLETAR conforme a especificacao) */

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

 /* reconhecimento de caracteres 'brancos'
  * - carriage return, line feed e tabs
  */
[ \r\n\t]+ { printf("\tBRANCO\n"); }

 /* lexemas complexos */
({minuscula}|_)({letra}|{digito}|_|#)*    { return(IDF); }
{maiuscula}({maiuscula}|{digito}|_)*      { return(CONST); }
{digito}+                                 { return(INT_LIT); }
{digito}+\.{digito}+([eE][+-]?{digito}+)? { return(F_LIT); }

 /* reconhecimento de comentarios
  * http://flex.sourceforge.net/manual/How-can-I-match-C_002dstyle-comments_003f.html
  */
<INITIAL>{
  "/*"              BEGIN(IN_COMMENT); printf("#COMM#\t%s", yytext);
}
<IN_COMMENT>{
  "*/"      BEGIN(INITIAL); printf("%s\n", yytext);
  [^*\n]+   printf("%s", yytext); // eat comment in chunks
  "*"       // eat the lone star
  \n        yylineno++; 
}

 /* Tratamento dos erros lexicais: a regra seguinte pega tudo o que nao
  * fechou com uma Regexp anterior.
  */
. { printf("#ERRO#\t%c\t%s\n", yytext[0], yytext); exit(-1); }

%%

 /* Secao dos  Procedimentos auxiliares  */

extern FILE *yyin;
int main(int argc, char* argv[]) {
  int token;
  if (argc != 2) {
    printf("uso: %s <input_file>. Try again!\n", argv[0]);
    exit(-1);
  }

  /* leitura de arquivo como entrada padrão */
  yyin = fopen(argv[1], "r");
  if (!yyin) {
    printf("Uso: %s <input_file>. Could not find %s. Try again!\n", argv[0], argv[1]);
    exit(-1);
  }

  printf("ID\tValue\n");
  printf("-----------------------------------------\n");
  while (token=yylex()) { 
    /* neste laco, obtem-se "palavra por palavra" os tokens reconhecidos
     * pelo scanner fornecido pelo Lex. Poderia ser feita a analise
     * sintatica... Sera feito depois!
     */
    printf("%d\t%s\n", token, yytext);
  }
  return(0);
}

