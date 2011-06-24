%{
	/* Aqui, pode-se inserir qualquer codigo C necessario ah compilacao
	* final do parser. Sera copiado tal como esta no inicio do y.tab.c
	* gerado por Yacc.
	*/
	#include <stdio.h>
	#include <stdlib.h>
	#include "node.h"
	#include "lista.h"
	#include "symbol_table.h"

	typedef struct _expr_attr {
		struct node_tac *code;
		char *local;
	}expr_attr;
	
	typedef struct _code_attr {
		struct node_tac *code;
	}code_attr;
	
	#define INT_TYPE	1
	#define FLOAT_TYPE  2
	#define DOUBLE_TYPE	3
	#define CHAR_TYPE	4
	
	//symbol_t symbol_table;

	char* novo_tmp(/*int type*/) {
		int type = FLOAT_TYPE;

		/* eu implementei o tmp em funcao do deslocamento nos
		   registradores, que depende do tipo do temporario.
		   Por enquanto eu deixei como float (tam 4), mas
		   provavelmente sera necessario criar um argumento. */

		static int mem = 0;
		int tamanho = 0;
		switch (type) {
			case INT_TYPE:    tamanho = 1; break;
			case FLOAT_TYPE:  tamanho = 4; break;
			case DOUBLE_TYPE: tamanho = 8; break;
			case CHAR_TYPE:   tamanho = 4; break;
			default:
				tamanho = 8;
		}
		char *ret = malloc(sizeof(char)*8);
		sprintf(ret, "%03d(Rx)", mem);
		
		mem += tamanho;
		return ret;

		// acho que nao eh necessario mexer com tabela de simbolos,
		// pois creio que temporarios vao em registradores... d(Rx)
		/*
		entry_t e;
		sprintf(e->name, "_TMP%03d", mem);
		e->type = type;
		e->size = tamanho;
		e->desloc = mem;   // mem + tamanho
		insert(&symbol_table, e); // symbol_table global
		return e->name;
		*/
	}
%}

%union {
  char* cadeia;
  struct _node * no;
}

%token<cadeia> IDF
%token<cadeia> CONST
%token<cadeia> INT
%token<cadeia> DOUBLE
%token<cadeia> FLOAT
%token<cadeia> REAL
%token<cadeia> CHAR
%token<cadeia> QUOTE
%token<cadeia> DQUOTE
%token<cadeia> LE
%token<cadeia> GE
%token<cadeia> EQ
%token<cadeia> NE
%token<cadeia> AND
%token<cadeia> OR
%token<cadeia> NOT
%token<cadeia> IF
%token<cadeia> THEN
%token<cadeia> ELSE
%token<cadeia> WHILE
%token<cadeia> FOR
%token<cadeia> NEXT
%token<cadeia> REPEAT
%token<cadeia> UNTIL
%token<cadeia> CASE
%token<cadeia> END
%token<cadeia> INT_LIT
%token<cadeia> F_LIT
%token<cadeia> TRUE
%token<cadeia> FALSE

%token<cadeia> ';'
%token<cadeia> ':'
%token<cadeia> ','
%token<cadeia> '['
%token<cadeia> ']'
%token<cadeia> '('
%token<cadeia> ')'
%token<cadeia> '{'
%token<cadeia> '}'
%token<cadeia> '='
%token<cadeia> '+'
%token<cadeia> '-'
%token<cadeia> '*'
%token<cadeia> '/'
%token<cadeia> '>'
%token<cadeia> '<'

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
					{ 
						  Node **c; 
						  pack_nodes(&c, 0, $1);
						  pack_nodes(&c, 0, $2);

							// Attribute synth
							code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
							cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
							cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);

						  $$ = create_node(0, program_node, "code", attr, 2, c);
						  syntax_tree = $$;
						}
    | acoes
					{
							$$ = $1; 
						  syntax_tree = $$;  
						}
    ;

declaracoes: declaracao ';'             { Node **c;
										  pack_nodes(&c, 0, $1);
										  pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
										  $$ = create_node(0, decl_node, "declaracoes", NULL, 2, c);
										}
           | declaracoes declaracao ';' { Node **c;
		   								  pack_nodes(&c, 0, $1);
										  pack_nodes(&c, 1, $2);
										  pack_nodes(&c, 2, create_leaf(0, semicolon_node, ";", NULL));
										  $$ = create_node(0, decl_node, "declaracoes", NULL, 3, c);
										}
           ;

declaracao: listadeclaracao ':' tipo { Node **c;
									   pack_nodes(&c, 0, $1);
									   pack_nodes(&c, 1, create_leaf(0, colon_node, ":", NULL));
									   pack_nodes(&c, 2, $3);
									   $$ = create_node(0, decl_node, "declaracao", NULL, 3, c);
									 }

listadeclaracao: IDF					 { $$ = create_leaf(0, idf_node, $1, NULL); } 
               | IDF ',' listadeclaracao { Node **c;
			   							   pack_nodes(&c, 0, create_leaf(0, idf_node, $1, NULL));
										   pack_nodes(&c, 1, create_leaf(0, comma_node, ",", NULL));
										   pack_nodes(&c, 2, $3);
										   $$ = create_node(0, decl_list_node, "lista declaracao", NULL, 3, c);
										 }
               ;

tipo: tipounico  { $$ = $1; }
    | tipolista  { $$ = $1; }
    ;

tipounico: INT    { $$ = create_leaf(0, int_node, "int", NULL); }
         | DOUBLE { $$ = create_leaf(0, double_node, "double", NULL); }
         | FLOAT  { $$ = create_leaf(0, float_node, "float", NULL); }
         | CHAR   { $$ = create_leaf(0, char_node, "char", NULL); }
		 | REAL   { $$ = create_leaf(0, real_node, "real", NULL); }
         ;

tipolista: INT '[' listadupla ']'    { Node **c;
									   pack_nodes(&c, 0, create_leaf(0, int_node, "int", NULL));
									   pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
									   pack_nodes(&c, 2, $3);
									   pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
									   $$ = create_node(0, tipolista_node, "tipo lista", NULL, 4, c);
									 }
         | DOUBLE '[' listadupla ']' { Node **c;
									   pack_nodes(&c, 0, create_leaf(0, double_node, "double", NULL));
									   pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
									   pack_nodes(&c, 2, $3);
									   pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
									   $$ = create_node(0, tipolista_node, "tipo lista", NULL, 4, c);
									 }
         | FLOAT '[' listadupla ']'  { Node **c;
									   pack_nodes(&c, 0, create_leaf(0, float_node, "float", NULL));
									   pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
									   pack_nodes(&c, 2, $3);
									   pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
									   $$ = create_node(0, tipolista_node, "tipo lista", NULL, 4, c);
									 }
         | CHAR '[' listadupla ']'   { Node **c;
									   pack_nodes(&c, 0, create_leaf(0, char_node, "char", NULL));
									   pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
									   pack_nodes(&c, 2, $3);
									   pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
									   $$ = create_node(0, tipolista_node, "tipo lista", NULL, 4, c);
									 }
		 | REAL '[' listadupla ']'   { Node **c;
									   pack_nodes(&c, 0, create_leaf(0, real_node, "real", NULL));
									   pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
									   pack_nodes(&c, 2, $3);
									   pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
									   $$ = create_node(0, tipolista_node, "tipo lista", NULL, 4, c);
									 }
         ;

listadupla: INT_LIT ':' INT_LIT       		   { Node **c;
									   		 	 pack_nodes(&c, 0, create_leaf(0, intlit_node, $1, NULL));
									   			 pack_nodes(&c, 1, create_leaf(0, colon_node, ":", NULL));
									   			 pack_nodes(&c, 2, create_leaf(0, intlit_node, $3, NULL));
									   			 $$ = create_node(0, tipolista_node, "lista dupla", NULL, 3, c);
									 		   }
          | INT_LIT ':' INT_LIT ',' listadupla { Node **c;
									   			 pack_nodes(&c, 0, create_leaf(0, intlit_node, $1, NULL));
									   			 pack_nodes(&c, 1, create_leaf(0, opencol_node, ":", NULL));
												 pack_nodes(&c, 2, create_leaf(0, intlit_node, $3, NULL));
									   			 pack_nodes(&c, 3, create_leaf(0, comma_node, ",", NULL));
												 pack_nodes(&c, 4, $5);
									   			 $$ = create_node(0, tipolista_node, "lista dupla", NULL, 5, c);
									 		   }
          ;

acoes: comando ';'		 { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
						   
						   // Attribute synth
						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));;
						   cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);

						   $$ = create_node(0, acoes_node, "acoes", attr, 2, c);
						 }
    | comando ';' acoes  { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
						   pack_nodes(&c, 2, $3);
						   
						   // Attribute synth
 						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						   cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
						   cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
						   
						   $$ = create_node(0, acoes_node, "acoes", attr, 3, c);
						 } 
    ;

comando: lvalue '=' expr { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, attrib_node, ":=", NULL));
						   pack_nodes(&c, 2, $3);
							
						   $$ = create_node(0, comando_node, "comando", NULL, 3, c);
						 } 
       | enunciado       { $$ = $1;}
       ;

lvalue: IDF                   { $$ = create_leaf(0, idf_node, $1, NULL); }
      | IDF '[' listaexpr ']' { Node **c;
							    pack_nodes(&c, 0, create_leaf(0, idf_node, $1, NULL));
							    pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
							    pack_nodes(&c, 2, $3);
							    pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));
							    $$ = create_node(0, tipolista_node, "lvalue", NULL, 4, c);
							  } 
      ;

listaexpr: expr             { $$ = $1; }
	   | expr ',' listaexpr { Node **c;
	   						  pack_nodes(&c, 0, $1);
							  pack_nodes(&c, 1, create_leaf(0, comma_node, ",", NULL));
							  pack_nodes(&c, 2, $3);
							  $$ = create_node(0, listaexpr_node, "lista expr", NULL, 3, c);
							}
	   ;

expr: expr '+' expr { // Pack nodes
					  Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, create_leaf(0, plus_node, "+", NULL));
					  pack_nodes(&c, 2, $3);
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = novo_tmp();
					  cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
 					  cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
					  struct tac *newcode = create_inst_tac(
					  	  attr->local,
					  	  ((expr_attr*)c[0]->attribute)->local,
						  "+",
						  ((expr_attr*)c[2]->attribute)->local
					  );
					  append_inst_tac(&attr->code, newcode);
					  
					  // Allocate new node
					  $$ = create_node(0, expr_node, "expr", attr, 3, c);
					}
    | expr '-' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, create_leaf(0, minus_node, "-", NULL));
					  pack_nodes(&c, 2, $3);
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = novo_tmp();
					  cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
 					  cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
					  struct tac *newcode = create_inst_tac(
					  	  attr->local,
 					  	  ((expr_attr*)c[0]->attribute)->local,
 					  	  "-",
					  	  ((expr_attr*)c[2]->attribute)->local
					  );
					  append_inst_tac(&attr->code, newcode);
					  
					  $$ = create_node(0, expr_node, "expr", attr, 3, c);
					}
    | expr '*' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, create_leaf(0, mult_node, "*", NULL));
					  pack_nodes(&c, 2, $3);
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = novo_tmp();
					  cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
 					  cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
					  struct tac *newcode = create_inst_tac(
					  	  attr->local,
 					  	  ((expr_attr*)c[0]->attribute)->local,
						  "*",
						  ((expr_attr*)c[2]->attribute)->local
					  );
					  append_inst_tac(&attr->code, newcode);
					  
					  $$ = create_node(0, expr_node, "expr", attr, 3, c);
					}
    | expr '/' expr { Node **c;
					  pack_nodes(&c, 0, $1);
					  pack_nodes(&c, 1, create_leaf(0, div_node, "/", NULL));
					  pack_nodes(&c, 2, $3);
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = novo_tmp();
					  cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
 					  cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
	 				  struct tac *newcode = create_inst_tac(
					  	  attr->local,
						  ((expr_attr*)c[0]->attribute)->local,
						  "/",
						  ((expr_attr*)c[2]->attribute)->local
					  );
					  append_inst_tac(&attr->code, newcode);
					  
					  $$ = create_node(0, expr_node, "expr", attr, 3, c);
					}
    | '(' expr ')'  { Node **c;
					  pack_nodes(&c, 0, create_leaf(0, openpar_node, "(", NULL));
					  pack_nodes(&c, 1, $2);
					  pack_nodes(&c, 2, create_leaf(0, closepar_node, ")", NULL));
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = ((expr_attr*)c[1]->attribute)->local;
					  cat_tac(&attr->code, &((expr_attr*)c[1]->attribute)->code);
					  
					  $$ = create_node(0, tipolista_node, "expr", attr, 3, c);
					}
    | INT_LIT       { // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));
					  attr->local = $1;
			    					  
					  $$ = create_leaf(0, intlit_node, $1, attr);
    				} 
    | F_LIT         { // Attribute synth
    				  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));
			    	  attr->local = $1;
    				  $$ = create_leaf(0, floatlit_node, $1, attr);
    				}
    | lvalue        { $$ = $1; }
    | chamaproc     { $$ = $1; }
    ;

chamaproc: IDF '(' listaexpr ')' { Node **c;
								   pack_nodes(&c, 0, create_leaf(0, idf_node, $2, NULL));
					  			   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
								   pack_nodes(&c, 2, $3);
					  			   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
								   create_node(0, proc_node, "chamaproc", NULL, 4, c);
								 }
         ;

enunciado: expr                                          { $$ = $1 ;}
         | IF '(' expbool ')' THEN acoes fiminstcontrole { Node **c;
														   pack_nodes(&c, 0, create_leaf(0, if_node, "if", NULL));
														   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
														   pack_nodes(&c, 2, $3);
														   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
														   pack_nodes(&c, 4, create_leaf(0, then_node, "then", NULL));
														   pack_nodes(&c, 5, $6);
														   pack_nodes(&c, 6, $7);
														   $$ = create_node(0, enunciado_node, "enunciado", NULL, 7, c);
														 }
         | WHILE '(' expbool ')' '{' acoes '}'           { Node **c;
														   pack_nodes(&c, 0, create_leaf(0, while_node, "while", NULL));
														   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
														   pack_nodes(&c, 2, $3);
														   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
														   pack_nodes(&c, 4, create_leaf(0, openbra_node, "{", NULL));
														   pack_nodes(&c, 5, $6);
														   pack_nodes(&c, 6, create_leaf(0, closebra_node, "}", NULL));
														   $$ = create_node(0, enunciado_node, "enunciado", NULL, 7, c);
														 }
         ;

fiminstcontrole: END            { $$ = create_leaf(0, end_node, "end", NULL); }
               | ELSE acoes END { Node **c;
								  pack_nodes(&c, 0, create_leaf(0, else_node, "else", NULL));
								  pack_nodes(&c, 1, $2);
								  pack_nodes(&c, 2, create_leaf(0, end_node, "end", NULL));
								  $$ = create_node(0, fimcontrole_node, "fiminstcontrole", NULL, 3, c);
								}
               ;

expbool: TRUE                 { $$ = create_leaf(0, true_node,  "true", NULL); }
       | FALSE                { $$ = create_leaf(0, false_node, "false", NULL); }
       | '(' expbool ')'      { Node **c;
							    pack_nodes(&c, 0, create_leaf(0, openpar_node, "(", NULL));
							    pack_nodes(&c, 1, $2);
							    pack_nodes(&c, 2, create_leaf(0, closepar_node, ")", NULL));
							    $$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | expbool AND expbool  { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, and_node, "and", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, and_node, "expbool", NULL, 3, c);
							  }
       | expbool OR expbool   { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, or_node, "or", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | NOT expbool          { Node **c; 
							    pack_nodes(&c, 0, create_leaf(0, not_node, "not", NULL));
	   						    pack_nodes(&c, 1, $2); 
								$$ = create_node(0, expbool_node, "expbool", NULL, 2, c); 
							  }
       | expr '>' expr        { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, sup_node, ">", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
	   | expr '<' expr        { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, inf_node, "<", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | expr LE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, inf_eq_node, "<=", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | expr GE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, sup_eq_node, ">=", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | expr EQ expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, eq_node, "=", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
							  }
       | expr NE expr         { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, neq_node, "!=", NULL));
								pack_nodes(&c, 2, $3);
								$$ = create_node(0, expbool_node, "expbool", NULL, 3, c);
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

	//init_table(&symbol_table);

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
	
	printf("Code:\n");
	print_tac(stdout, ((code_attr*)syntax_tree->attribute)->code);
	
	return(0);
}

yyerror(char* s) {
  fprintf(stderr, "%s: %s", progname, s);
  fprintf(stderr, "line %d\n", lineno);
}


