%{
  /* Aqui, pode-se inserir qualquer codigo C necessario ah compilacao
   * final do parser. Sera copiado tal como esta no inicio do y.tab.c
   * gerado por Yacc.
   */
  #include <stdio.h>
  #include <stdlib.h>
  #include "node.h"

%}

%union {
  char* cadeia;
  struct _node * no;
}

%token<no> IDF
%token<no> CONST
%token<no> INT
%token<no> DOUBLE
%token<no> FLOAT
%token<no> REAL
%token<no> CHAR
%token<no> QUOTE
%token<no> DQUOTE
%token<no> LE
%token<no> GE
%token<no> EQ
%token<no> NE
%token<no> AND
%token<no> OR
%token<no> NOT
%token<no> IF
%token<no> THEN
%token<no> ELSE
%token<no> WHILE
%token<no> FOR
%token<no> NEXT
%token<no> REPEAT
%token<no> UNTIL
%token<no> CASE
%token<no> END

%token<cadeia> INT_LIT
%token<cadeia> F_LIT
%token<cadeia> TRUE
%token<cadeia> FALSE

%type<no> code
%type<no> declaracoes 
%type<no> declaracao
%type<no> listadeclaracao
%type<no> tipo
%type<no> tipounico
%type<no> tipolista
%type<no> listadupla
%type<no> acoes
%type<no> comando
%type<no> lvalue
%type<no> listaexpr
%type<no> expr
%type<no> chamaproc
%type<no> enunciado
%type<no> fiminstcontrole
%type<no> expbool

%start code

%%

code: declaracoes acoes
    | acoes { $$ = $1; syntax_tree = $$;  }
    ;

declaracoes: declaracao ';'
           | declaracoes declaracao ';'
           ;

declaracao: listadeclaracao ':' tipo

listadeclaracao: IDF
               | IDF ',' listadeclaracao
               ;

tipo: tipounico 
    | tipolista
    ;

tipounico: INT
         | DOUBLE
         | FLOAT
         | CHAR
         ;

tipolista: INT '[' listadupla ']'
         | DOUBLE '[' listadupla ']'
         | FLOAT '[' listadupla ']'
         | CHAR '[' listadupla ']'
         ;

listadupla: INT_LIT ':' INT_LIT
          | INT_LIT ':' INT_LIT ',' listadupla
          ;

acoes: comando ';'  { $$ = $1; }
    | comando ';' acoes
    ;

comando: lvalue '=' expr
       | enunciado { $$ = $1;}
       ;

lvalue: IDF
      | IDF '[' listaexpr ']'
      ;

listaexpr: expr             { $$ = $1; }
	   | expr ',' listaexpr { Node **c;
	   						  pack_nodes(&c, 0, $1);
							  pack_nodes(&c, 0, $3);
							  $$ = create_node(0, decl_list_node, "DECL_LIST", NULL, 2, c);
							}
	   ;

expr: expr '+' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, $3);
					  $$ = create_node(0, plus_node, "SOMA", NULL, 2, c);
					}
    | expr '-' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, $3);
					  $$ = create_node(0, minus_node, "SUBTRACAO", NULL, 2, c);
					}
    | expr '*' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, $3);
					  $$ = create_node(0, mult_node, "MULTIPLICACAO", NULL, 2, c);
					}
    | expr '/' expr  { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, $3);
					  $$ = create_node(0, div_node, "DIVISAO", NULL, 2, c);
					}
    | '(' expr ')'  { $$ = $2; }
    | INT_LIT       { $$ = create_leaf(0, int_node,   $1, NULL); } 
    | F_LIT         { $$ = create_leaf(0, float_node, $1, NULL); }
    | lvalue        { $$ = $1; }
    | chamaproc     { $$ = $1; }
    ;

chamaproc: IDF '(' listaexpr ')' { Node **c;
								   pack_nodes(&c, 0, $1);
								   pack_nodes(&c, 0, $3);
								   create_node(0, proc_node, "PROC", NULL, 2, c);
								 }
         ;

enunciado: expr                                          { $$ = $1 ;}
         | IF '(' expbool ')' THEN acoes fiminstcontrole
         | WHILE '(' expbool ')' '{' acoes '}'
         ;

fiminstcontrole: END            { /*$$ = create_leaf(0, end_node, "END", NULL); precisa ser tratado?*/ }
               | ELSE acoes END { Node **c;
								  pack_nodes(&c, 0, $2);
								  $$ = create_node(0, else_node, "ELSE", NULL, 1, c);
								}
               ;

expbool: TRUE                 { $$ = create_leaf(0, true_node,  $1, NULL); }
       | FALSE                { $$ = create_leaf(0, false_node, $1, NULL); }
       | '(' expbool ')'      { $$ = $2; }
       | expbool AND expbool  { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, and_node, "AND", NULL, 2, c);
							  }
       | expbool OR expbool   { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, or_node, "OR", NULL, 2, c);
							  }
       | NOT expbool          { Node **c; 
	   						    pack_nodes(&c, 0, $1); 
								$$ = create_node(0, not_node, "NOT", NULL, 1, c); 
							  }
       | expr '>' expr        { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, sup_node, "GT", NULL, 2, c);
							  }
       | expr '<' expr        { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, inf_node, "LT", NULL, 2, c);
							  }
       | expr LE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, inf_eq_node, "LE", NULL, 2, c);
							  }
       | expr GE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, sup_eq_node, "GE", NULL, 2, c);
							  }
       | expr EQ expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, eq_node, "EQ", NULL, 2, c);
							  }
       | expr NE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
								pack_nodes(&c, 0, $3);
								$$ = create_node(0, neq_node, "NE", NULL, 2, c);
							  }
       ;
%%
 /* A partir daqui, insere-se qlqer codigo C necessario.
  */

char* progname;
int lineno;
extern FILE* yyin;

int main(int argc, char* argv[]) 
{
	if (argc != 2) {
		printf("uso: %s <input_file>. Try again!\n", argv[0]);
		exit(-1);
	}
	yyin = fopen(argv[1], "r");
	if (!yyin) {
		printf("Uso: %s <input_file>. Could not find %s. Try again!\n", argv[0], argv[1]);
		exit(-1);
	}

	progname = argv[0];

	if (!yyparse()) 
		printf("OKAY.\n");
	else 
		printf("ERROR.\n");

	/*switch(syntax_tree->type) {
	case int_node: 
		printf("A AST se limita a uma folha rotulada por: %s\n", syntax_tree->lexeme);
		break;
	case plus_node:
		printf("Soma de %s com %s.\n", syntax_tree->children[0]->lexeme, syntax_tree->children[1]->lexeme);
		break;
	case minus_node:
		printf("Subtracao de %s com %s.\n", syntax_tree->children[0]->lexeme, syntax_tree->children[1]->lexeme);
		break;
	}*/

	printf("Arvore final (altura %i):\n", height(syntax_tree));
	printTree(syntax_tree);

	return(0);
}

yyerror(char* s) {
  fprintf(stderr, "%s: %s", progname, s);
  fprintf(stderr, "line %d\n", lineno);
}
