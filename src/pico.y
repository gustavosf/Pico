%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "node.h"
	#include "lista.h"
	#include "symbol_table.h"

	typedef struct _expr_attr {
		struct node_tac *code;
		char *local;
	}expr_attr;
	
	typedef struct _code_attr {
		struct node_tac *code;
		char *next;
	}code_attr;
	
	// tipos para declaracoes de tipo
	typedef struct _tipo_attr {
		int size;
		int type;
	} tipo_attr;

	typedef struct _listadupla_attr {
		int first; // primeiro elemento da lista
		int size;  // número de elementos da lista
	} listadupla_attr;

	typedef struct _tipolista_attr {
		int tipo;            // tipo dos elementos da lista
		int size;            // tamanho dos elementos da lista (ld->size * node_size(tipo))
		listadupla_attr *ld; // informacoes sobre a lista
	} tipolista_attr;

	// tipos booleanos
	typedef struct _tipobool_attr {
		struct node_tac *code;
		char *t;
		char *f;
	} tipobool_attr;
	
	// tipo while
	typedef struct _while_attr {
		struct node_tac *code;
		char *next;
		char *begin;
	}while_attr;
	
	#define INT_TYPE	1
	#define FLOAT_TYPE  2
	#define DOUBLE_TYPE	3
	#define CHAR_TYPE	4
	#define REAL_TYPE   5

	symbol_t symbol_table;
	int memoria = 0;
	int temps = 0;

	char* novo_tmp(/*int type*/) {
		int type = INT_TYPE;

		int tamanho = 0;
		switch (type) {
			case INT_TYPE:    tamanho = 4; break;
			case FLOAT_TYPE:  tamanho = 4; break;
			case DOUBLE_TYPE: tamanho = 8; break;
			case REAL_TYPE:   tamanho = 8; break;
			case CHAR_TYPE:   tamanho = 1; break;
			default:
				tamanho = 8;
		}
		char *ret = malloc(sizeof(char)*8);
		sprintf(ret, "%03d(Rx)", temps);
		
		temps += tamanho;
		return ret;
	}

	// "Tratamento" de erro
	#define UNDEFINED_SYM_ERROR  -100
	#define OUT_OF_RANGE_ERROR   -101
	#define SYM_REDECLARED_ERROR -102
	int picoerror(int error) {
		if (error == UNDEFINED_SYM_ERROR)
			fprintf(stderr, "Variavel nao declarada\n");
		if (error == OUT_OF_RANGE_ERROR)
			fprintf(stderr, "Acesso ilegal ao array\n");
		if (error == SYM_REDECLARED_ERROR)
			fprintf(stderr, "Variavel ja foi declarada\n");
		return error;
	}

	int node_type(Node_type t) {
		switch (t) {
			case int_node:    return INT_TYPE;
			case float_node:  return FLOAT_TYPE;
			case double_node: return DOUBLE_TYPE;
			case real_node:   return REAL_TYPE;
			case char_node:   return CHAR_TYPE;
			default:          return REAL_TYPE;
		}
	}

	int node_size(Node_type t) {
		switch (t) {
			case int_node:    return 4;
			case float_node:  return 4;
			case double_node: return 8;
			case real_node:   return 8;
			case char_node:   return 1;
			default:          return 8;
		}
	}

	void insert_decl(Node* dec, Node* tipo) {
		if (dec->type == idf_node) {		
			if (lookup(symbol_table, dec->lexeme) == NULL) {
				entry_t *e = malloc(sizeof(entry_t));
				e->name = malloc(sizeof(char)*(strlen(dec->lexeme) + 1));
				strcpy(e->name, dec->lexeme);

				if ((tipo->type == int_node) ||
				    (tipo->type == float_node) ||
				    (tipo->type == double_node) ||
				    (tipo->type == char_node) ||
				    (tipo->type == real_node)) { 
				    // tipos unicos
				    e->type = node_type(tipo->type);
				    e->size = node_size(tipo->type);
					e->extra = NULL;
				} else if (tipo->type == tipolista_node) {
					e->type = ((tipolista_attr *)tipo->attribute)->tipo;
					e->size = ((tipolista_attr *)tipo->attribute)->size;
					e->extra = ((tipolista_attr *)tipo->attribute)->ld;
				}
				e->desloc = memoria;
				memoria += e->size;
				insert(&symbol_table, e);
			} else {
				picoerror(SYM_REDECLARED_ERROR);
				exit(-1);
			}
		} else {
			int i;
			Node *child;
			for (i = 0; i < dec->nb_children; i++)
				if (dec->children != NULL)
					insert_decl(dec->children[i], tipo);
		}
	}

	/* Expressoes booleanas */
	char* novo_rotulo() {
		static int rotulo = 0;
		char *ret = malloc(sizeof(char)*8);
		sprintf(ret, "LABEL%03d", rotulo++);
		return ret;
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
%type<no> operbool

// precedencia de operadores (vide spec.pg25)
%left '+' '-'
%left '*' '/'
%left OR
%left AND
%left NOT

%start code

%%

code: declaracoes acoes { Node **c; 
						  pack_nodes(&c, 0, $1);
						  pack_nodes(&c, 1, $2);

						  // Attribute synth
						  code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						  //cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
						  cat_tac(&attr->code, &((expr_attr*)c[1]->attribute)->code);

						  $$ = create_node(0, program_node, "code", attr, 2, c);
						  syntax_tree = $$;
						}
    | acoes 			{ $$ = $1;
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
										  $$ = create_node(0, decl_node, "declaracoes", NULL, 2, c);
										}
           ;

declaracao: listadeclaracao ':' tipo { Node **c, *n;
									   insert_decl($1, $3);
									   print_table(symbol_table);
									   pack_nodes(&c, 0, $1);
									   pack_nodes(&c, 1, $3);
									   $$ = create_node(0, decl_node, "declaracao", NULL, 2, c);
									 }

listadeclaracao: IDF					 { $$ = create_leaf(0, idf_node, $1, NULL); } 
               | IDF ',' listadeclaracao { Node **c;
			   							   pack_nodes(&c, 0, create_leaf(0, idf_node, $1, NULL));
										   pack_nodes(&c, 1, $3);
										   $$ = create_node(0, decl_list_node, "lista declaracao", NULL, 2, c);
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

tipolista: tipounico '[' listadupla ']'{ Node **c;
									   	 tipolista_attr *l = malloc(sizeof(tipolista_attr));
									   	 listadupla_attr *ld = $3->attribute;
									   	 l->tipo = node_type($1->type);
									   	 l->ld = ld;
									   	 l->size = node_size($1->type) * ld->size;
										 pack_nodes(&c, 0, $1);
									   	 pack_nodes(&c, 1, $3);
									   	 $$ = create_node(0, tipolista_node, "tipo lista", l, 2, c);
									 }
		 /* compactados na producao acima
         | DOUBLE '[' listadupla ']' { }
         | FLOAT '[' listadupla ']'  { }
         | CHAR '[' listadupla ']'   { }
		 | REAL '[' listadupla ']'   { }*/
         ;

listadupla:	INT_LIT ':' INT_LIT { Node **c;
								  listadupla_attr *l = malloc(sizeof(listadupla_attr));
								  l->first = atoi($1);
								  l->size = atoi($3) - l->first + 1;
								  pack_nodes(&c, 0, create_leaf(0, intlit_node, $1, NULL));
								  pack_nodes(&c, 1, create_leaf(0, intlit_node, $3, NULL));
								  $$ = create_node(0, listadupla_node, "lista dupla", l, 2, c);
								}
		  | INT_LIT				{ Node **c;
		  						  listadupla_attr *l = malloc(sizeof(listadupla_attr));
								  l->first = 0;
								  l->size = atoi($1) + 1;
								  $$ = create_leaf(0, listadupla_node, $1, l);
								}
		  /* Não será implementado (arrays multidimensionais)
          | INT_LIT ':' INT_LIT ',' listadupla { } */
          ;

acoes: comando ';'		 { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));

						   // Attribute synth
						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						   cat_tac(&attr->code, &((code_attr*)c[0]->attribute)->code);
						   
						   $$ = create_node(0, acoes_node, "acoes", attr, 2, c);
						 }
    | comando ';' acoes  { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
						   pack_nodes(&c, 2, $3);
						   
						   // Attribute synth
 						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						   cat_tac(&attr->code, &((code_attr*)c[0]->attribute)->code);
						   cat_tac(&attr->code, &((code_attr*)c[2]->attribute)->code);
						   
						   $$ = create_node(0, acoes_node, "acoes", attr, 3, c);
						 } 
    ;

comando: lvalue '=' expr { Node **c;
						   pack_nodes(&c, 0, $1);
						   pack_nodes(&c, 1, create_leaf(0, attrib_node, ":=", NULL));
						   pack_nodes(&c, 2, $3);

						   // Attribute synth
					       expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  	   attr->local = ((expr_attr*)c[0]->attribute)->local;
						   cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
						   cat_tac(&attr->code, &((expr_attr*)c[2]->attribute)->code);
						   struct tac *newcode = create_inst_tac(
						   	   ((expr_attr*)c[0]->attribute)->local,
						   	   ((expr_attr*)c[2]->attribute)->local,
						  	   ":=",
						  	   ""
					  	   );
						   append_inst_tac(&attr->code, newcode);

						   $$ = create_node(0, comando_node, "comando", attr, 3, c);
						 } 
       | enunciado       { $$ = $1;}
       ;

lvalue: IDF                   { entry_t *ref = lookup(symbol_table, $1);
								if (ref == NULL) return picoerror(UNDEFINED_SYM_ERROR);
								expr_attr *attr = (expr_attr*)malloc(sizeof(expr_attr));
					  			attr->local = malloc(sizeof(char) * 8);
					  			sprintf(attr->local, "%03d(SP)", ref->desloc);
								$$ = create_leaf(0, idf_node, $1, attr); 
							  }
      | IDF '[' listaexpr ']' { Node **c;
							    pack_nodes(&c, 0, create_leaf(0, idf_node, $1, NULL));
							    pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
							    pack_nodes(&c, 2, $3);
							    pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));

							    entry_t *ref = lookup(symbol_table, $1);
								if (ref == NULL) return picoerror(UNDEFINED_SYM_ERROR);
								
								expr_attr *i = (expr_attr*)c[2]->attribute;

								/*
								int size = ((listadupla_attr *)ref->extra)->size;
								int first = ((listadupla_attr *)ref->extra)->first;
								int i = atoi((char *)((expr_attr*)c[2]->attribute)->local) - first + 1;
								if (i < 0 || i >= size) return picoerror(OUT_OF_RANGE_ERROR);

								int desloc;
								switch (ref->type) {
									case INT_TYPE:    desloc = ref->desloc + i * 1; break;
									case FLOAT_TYPE:  desloc = ref->desloc + i * 4; break;
									case CHAR_TYPE:   desloc = ref->desloc + i * 4; break;
									case REAL_TYPE:   desloc = ref->desloc + i * 8; break;
									case DOUBLE_TYPE: desloc = ref->desloc + i * 8; break;
								}

								expr_attr *attr = (expr_attr*)malloc(sizeof(expr_attr));
					  			attr->local = malloc(sizeof(char) * 8);
					  			sprintf(attr->local, "%03d(SP)", desloc);
								
							    $$ = create_node(0, idf_node, "lvalue", attr, 4, c);
							    */

							    expr_attr *attr = (expr_attr*)malloc(sizeof(expr_attr));
					  			
					  			char *res = novo_tmp();
					  			char *desloc = malloc(sizeof(char)*12);
					  			sprintf(desloc, "%d", ref->desloc);

							    append_inst_tac(&attr->code, create_inst_tac(res,i->local,":=",""));
							    append_inst_tac(&attr->code, create_inst_tac(res, res, "ADD", desloc));
							    attr->local = malloc(sizeof(char) * 17);
					  			strcpy(attr->local, res);
					  			strcat(attr->local, "(000(SP))");
								$$ = create_node(0, idf_node, "lvalue", attr, 4, c);
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

enunciado: expr                                          { $$ = $1; }
         | IF '(' expbool ')' THEN acoes fiminstcontrole {
															int hasElse = ($7->type == fimcontrole_node);
															printf("hasElse? %i\n", hasElse);
															
															Node **c;
															pack_nodes(&c, 0, create_leaf(0, if_node, "if", NULL));
															pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
															pack_nodes(&c, 2, $3); //B
															pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
															pack_nodes(&c, 4, create_leaf(0, then_node, "then", NULL));
															pack_nodes(&c, 5, $6); //S1
															pack_nodes(&c, 6, $7); //S2


															// Attribute synth	
															code_attr *attr = (code_attr*) malloc(sizeof(code_attr));

															//B.t = novo_rot
															((tipobool_attr*)$3->attribute)->t = novo_rotulo(); //should be updating the '#' in "IF xxx GOTO LABEL#", but is not working (why?).
															
															// calcula S.next...
															attr->next = novo_rotulo();//**********************TODO**********************
															
															if(hasElse)
																((tipobool_attr*)$3->attribute)->f = novo_rotulo();
															else
																((tipobool_attr*)$3->attribute)->f = attr->next;

															//S1.next = S.next
															((code_attr*)$6->attribute)->next = attr->next;
															
															//S1 and S2 attribute synth
															if(hasElse) {
																((code_attr*)$7->attribute)->next = attr->next; //S2.next = S.next
																
																cat_tac(&attr->code, &((tipobool_attr*)$3->attribute)->code); //B.code
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)($3->attribute))->t, "", "", "") ); //geracod(B.t)
																cat_tac(&attr->code, &((code_attr*)$6->attribute)->code); //S1.code														   
																append_inst_tac(&attr->code, create_inst_tac( "GOTO",attr->next,"","") ); //geracod('goto' S.next)
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)($3->attribute))->f, "", "", "") ); //geracod(B.f)
																cat_tac(&attr->code, &((code_attr*)$7->attribute)->code); //S2.code												
															}
															else {
																cat_tac(&attr->code, &((tipobool_attr*)$3->attribute)->code); //B.code
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)($3->attribute))->t, "", "", "") ); //geracod(B.t)
																cat_tac(&attr->code, &((code_attr*)$6->attribute)->code); //S1.code														   
															}
															append_inst_tac(&attr->code, create_inst_tac( attr->next,"","","") ); //geracod(S.next)
															

															$$ = create_node(0, enunciado_node, "enunciado", attr, 7, c);
														 }
         | WHILE '(' expbool ')' '{' acoes '}'           { Node **c;
														   pack_nodes(&c, 0, create_leaf(0, while_node, "while", NULL));
														   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
														   pack_nodes(&c, 2, $3); //B
														   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
														   pack_nodes(&c, 4, create_leaf(0, openbra_node, "{", NULL));
														   pack_nodes(&c, 5, $6); //S1
														   pack_nodes(&c, 6, create_leaf(0, closebra_node, "}", NULL));
														   
														   // Attribute synth	
															while_attr *attr = (while_attr*) malloc(sizeof(while_attr));

															// calcula S.next
															attr->next = novo_rotulo(); //**********************TODO**********************

															((tipobool_attr*)$3->attribute)->f = attr->next;
															((tipobool_attr*)$3->attribute)->t = novo_rotulo();

															attr->begin = novo_rotulo(); //S.begin = novo_rot
															((while_attr*)$6->attribute)->next = attr->begin; //S1.next = S.begin

															append_inst_tac(&attr->code, create_inst_tac( attr->begin,"","","") ); //geracod(S.begin)
															cat_tac(&attr->code, &((tipobool_attr*)$3->attribute)->code); //B.code	
															append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)($3->attribute))->t,"","","") ); //geracod(B.t)
															cat_tac(&attr->code, &((code_attr*)$6->attribute)->code); //S1.code	
															append_inst_tac(&attr->code, create_inst_tac( "GOTO",attr->begin,"","") ); //geracod('goto' S.begin)
															append_inst_tac(&attr->code, create_inst_tac( attr->next,"","","") ); //geracod(S.begin)

														   
														   $$ = create_node(0, enunciado_node, "enunciado", attr, 7, c);
														 }
         ;

fiminstcontrole: END            { $$ = create_leaf(0, end_node, "end", NULL); }
               | ELSE acoes END { Node **c;
								  pack_nodes(&c, 0, create_leaf(0, else_node, "else", NULL));
								  pack_nodes(&c, 1, $2);
								  pack_nodes(&c, 2, create_leaf(0, end_node, "end", NULL));
								  
								  code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
									cat_tac(&attr->code, &((code_attr*)$2->attribute)->code);
								  
								  $$ = create_node(0, fimcontrole_node, "fiminstcontrole", attr, 3, c);
								}
               ;

expbool: TRUE                 { tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								attr->t = novo_rotulo();
								attr->f = novo_rotulo();
								append_inst_tac(&attr->code, create_inst_tac("GOTO",attr->t,"",""));
								$$ = create_leaf(0, true_node, "true", attr);
							  }
       | FALSE                { tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								attr->t = novo_rotulo();
								attr->f = novo_rotulo();
								append_inst_tac(&attr->code, create_inst_tac("GOTO",attr->f,"",""));
								$$ = create_leaf(0, false_node, "false", attr);
							  }
       | '(' expbool ')'      { $$ = $2; }
       | expbool AND expbool  { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, and_node, "and", NULL));
								pack_nodes(&c, 2, $3);
								
								// Attr synth
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								tipobool_attr *b1 = ((tipobool_attr *)$1->attribute);
         						tipobool_attr *b2 = ((tipobool_attr *)$3->attribute);

								attr->f = b1->f;
								attr->t = b2->t;

								cat_tac(&attr->code, &b1->code);
								append_inst_tac(&attr->code,create_inst_tac(b1->t, "", "", ""));
								cat_tac(&attr->code, &b2->code);
								$$ = create_node(0, and_node, "expbool", attr, 3, c);
							  }
       | expbool OR expbool   { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, and_node, "or", NULL));
								pack_nodes(&c, 2, $3);
								
								// Attr synth
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								tipobool_attr *b1 = ((tipobool_attr *)$1->attribute);
         						tipobool_attr *b2 = ((tipobool_attr *)$3->attribute);

								attr->f = b1->f;
								attr->t = b2->t;

								cat_tac(&attr->code, &b1->code);
								append_inst_tac(&attr->code,create_inst_tac(b1->f, "", "", ""));
								cat_tac(&attr->code, &b2->code);
								$$ = create_node(0, or_node, "expbool", attr, 3, c);
							  }
       | NOT expbool          { Node **c; 
							    pack_nodes(&c, 0, create_leaf(0, not_node, "not", NULL));
	   						    pack_nodes(&c, 1, $2); 
	   						    // inverter os labels... (como?)
								$$ = create_node(0, expbool_node, "expbool", NULL, 2, c); 
							  }
	   | expr operbool expr       { Node **c;
	   							pack_nodes(&c, 0, $1);
							    pack_nodes(&c, 1, create_leaf(0, op_node, $2->lexeme, NULL));
								pack_nodes(&c, 2, $3);
								
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								expr_attr *e1 = ((expr_attr *)$1->attribute);
								expr_attr *e2 = ((expr_attr *)$3->attribute);

								attr->t = novo_rotulo();
								attr->f = novo_rotulo();

								cat_tac(&attr->code, &e1->code);
								cat_tac(&attr->code, &e2->code);
								append_inst_tac(&attr->code,create_inst_tac(attr->t, e1->local, ">", e2->local));
								append_inst_tac(&attr->code,create_inst_tac("GOTO", attr->f, "", ""));
								$$ = create_node(0, op_node, "expbool", attr, 3, c);
	   						  }
	   /* Compactado na produção acima
       | expr '>' expr        { }
	   | expr '<' expr        { }
       | expr LE expr         { }
       | expr GE expr         { }
       | expr EQ expr         { }
       | expr NE expr         { }*/
       ;
operbool: '>' { $$ = create_leaf(0, sup_node, ">", NULL); }
		| '<' { $$ = create_leaf(0, inf_node, "<", NULL); }
		| GE  { $$ = create_leaf(0, sup_eq_node, ">=", NULL); }
		| LE  { $$ = create_leaf(0, inf_eq_node, "<=", NULL); }
		| EQ  { $$ = create_leaf(0, eq_node, "==", NULL); }
		| NE  { $$ = create_leaf(0, neq_node, "!=", NULL); }
		;
%%

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

	init_table(&symbol_table);

	if (!yyparse()) printf("OKAY.\n");
	else printf("ERROR.\n");

	printf("Arvore final (altura %i):\n", height(syntax_tree));
	printTree(syntax_tree);
	
	printf("Code:\n");
	printf("%d\n%d\n", memoria, temps);
	print_tac(stdout, ((code_attr*)syntax_tree->attribute)->code);
	
	return(0);
}

yyerror(char* s) {
  fprintf(stderr, "%s: %s", progname, s);
  fprintf(stderr, "line %d\n", lineno);
}