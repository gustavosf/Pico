/** @file lista.c
 * \version 1
 * \author Daniel Beck
 * \author Francieli Zanon
 * \author Joseane Barrios
 * \brief Implementacao de uma lista duplamente encadeada
 */

#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _LISTA_C_
#define _LISTA_C_

/** \param[in] res alguma coisa que não sabemos
 * \param[in] arg1 primeiro argumento
 * \param[in] op operação
 * \param[in] arg2 segundo argumento
 * \return ponteiro para uma instrução
 */
struct tac* create_inst_tac(const char* res, const char* arg1, 
                 const char* op, const char* arg2) {
   struct tac * t = (struct tac *)malloc(sizeof(struct tac));
   t->op = (char*)malloc(sizeof(char)*(1+strlen(op)));
   strcpy(t->op, op);
   t->res = (char*)malloc(sizeof(char)*(1+strlen(res)));
   strcpy(t->res, res);
   t->arg1 = (char*)malloc(sizeof(char)*(1+strlen(arg1)));
   strcpy(t->arg1, arg1);
   t->arg2 = (char*)malloc(sizeof(char)*(1+strlen(arg2)));
   strcpy(t->arg2, arg2);
   return( t );
}

/** \param[out] out arquivo de destino
 * \param[in] i instrução a ser escrita
 * \return nada
 */
void print_inst_tac(FILE* out, struct tac i) {
   fprintf(out, "%s\t:= %s %s %s\n", i.res, i.arg1, i.op, i.arg2);
}

/** \param[out] out arquivo de destino
 * \param[in] code lista contendo as instruções
 * \return nada
 */
void print_tac(FILE* out, struct node_tac* code) {
    struct tac* aux_tac;
    while(code != NULL) {
	aux_tac = code->inst;
	print_inst_tac(out, *aux_tac);
	code = code->next;
    }
}

/** \param[out] code lista de destino
 * \param[in] inst instrução a ser inserida
 * \return nada
 */
void append_inst_tac(struct node_tac** code, struct tac* inst) {
    struct node_tac* new_node = (struct node_tac*)malloc(sizeof(struct node_tac));
    new_node->inst = inst;
    new_node->next = NULL;
    if (*code == NULL) { /** lista vazia */
	new_node->prev = NULL;
	*code = new_node;
    } else { /** lista nao vazia */
	struct node_tac* node = *code;
	while(node->next != NULL) {
	    node = node->next;
	};
	node->next = new_node;
	new_node->prev = node;
    };
}

/** \param[out] code_a lista de destino
 * \param[in] code_b lista a ser concatenada com a lista de destino
 * \return nada
 */
void cat_tac(struct node_tac** code_a, struct node_tac** code_b) {
    if (*code_b != NULL) { /** se a segunda lista for vazia, nao faz nada */
	if (*code_a == NULL) { /** se a primeira lista for vazia, simplesmente altera o ponteiro */
	    *code_a = *code_b;
	} else { /** se ambas as listas forem nao vazias, faz concatenacao propriamente dita */
	    struct node_tac* aux_node = *code_a;
	    while(aux_node->next != NULL) {
		aux_node = aux_node->next;
	    };
	    aux_node->next = *code_b;
	};
    };
}

#endif
