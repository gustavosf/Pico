/** @file node.c
 * \version 1
 * \author Daniel Beck
 * \author Francieli Zanon
 * \author Joseane Barrios
 * \brief Implementacao de uma arvore n-aria
 */

#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pode ser util...*/
static int __nodes_ids__ = 0;

Node * syntax_tree;


void notify_error(int tipo)
{
    switch(tipo)
    {
        case NARG:
            printf("Argumento inválido, não pode ser NULL!\n");
            break;
        case OLARG:
            printf("Argumento inválido, fora da faixa correta!\n");
            break;
    }
    exit(-1);
}
Node* create_node(int nl, Node_type t, char* lex, 
                  void* att, int nbc, Node** children) {
    int i; /*auxiliar para laço*/
    /*verifica se o número de filhos está dentro do permitido*/
    if((nbc < 0) || (nbc > MAX_CHILDREN_NUMBER))
        notify_error(OLARG);
    /*aloca o espaço para ele e o inicializa*/
    Node *novo = create_leaf(nl, t, lex, att);
    novo->nb_children = nbc;
    novo->children=children;
    /*retorna o nodo*/
    return novo;
}

Node* create_leaf(int nl, Node_type t, char* lex, void* att) {
    /*verifica o tipo*/
    if((t < 299) || (t > 336))
        notify_error(OLARG);
    /*aloca espaço para o novo nó*/
    Node *novo = malloc(sizeof(Node));
    /*inicializa ele*/
    novo->num_line = nl;
    novo->id = __nodes_ids__;
    __nodes_ids__++;
        /*copia o string*/
    if(lex == NULL) 
        novo->lexeme = NULL;
    else
    {
        novo->lexeme = malloc(sizeof(char)*(strlen(lex) + 1));
        strncpy(novo->lexeme, lex, strlen(lex));
        novo->lexeme[strlen(lex)] = '\0';
    }
        /*fim cópia string*/
    novo->type = t;
    novo->attribute = att;
    /*determina que ele não tem filhos*/
    novo->nb_children = 0;
    novo->children=NULL;
    /*retorna ele*/
    return novo;
}

int nb_of_children(Node* n) {
    /*verifica se o argumento é NULL*/
    if(n == NULL)
        notify_error(NARG);
    /*retorna o número de filhos*/
    return n->nb_children;
}


Node* child(Node* n, int i) {
    int j; /*auxiliar para laço*/
    /*verifica se n é NULL. Se for, retorna um erro*/
    if(n == NULL)
        notify_error(NARG);
    /*verifica se i está no intervalo correto. Se não estiver, retorna um
     * erro*/
    if((i < 0) || (i >= n->nb_children))
        notify_error(OLARG);
    /*retorna o filho desejado*/
    return n->children[i];
}

int deep_free_node(Node* n) {
   int ret=0; /*retorno da função*/
   int i; /*auxiliar para laço*/
   /*verifica se há algo para liberar*/
   if(n == NULL)
       return 0;
   /*libera todos os filhos*/
   if(n->nb_children > 0)
       for(i=0; i< MAX_CHILDREN_NUMBER; i++)
           ret +=deep_free_node(n->children[i]);
   /*apaga esse próprio nó*/
    free(n);
    return ret;
}

int height(Node *n) {
    int max=0; /*maior altura entre as subárvores filhas*/
    int alt; /*auxiliar para guardar altura da subárvore filha*/
    int i; /*auxiliar para laço*/
   /*a altura de uma árvore vazia é 0*/
    if(n == NULL)
        return 0;
    /*calcula a altura abaixo desse nó*/
    for(i=0; i < n->nb_children; i++)
    {
        /*pega a altura dessa subárvore*/
        alt = height(n->children[i]);
        /*vê se é maior que a máxima*/
        if(max < alt) 
            max = alt;
    }
    /*retorna a altura total dessa árvore: 1 + a altura da maior subárvore*/
    return 1 + max;
}

int pack_nodes(Node*** array_of_nodes, const int cur_size, Node* n) {
    if(array_of_nodes == NULL)
        notify_error(NARG);
    int i; /*auxiliar para laços*/
    /*verifica se o cur_size está dentro do limite*/
    if((cur_size < 0) || (cur_size >= MAX_CHILDREN_NUMBER))
        notify_error(OLARG);
    /*verifica se é a inicialização da lista*/
    if(cur_size == 0)
    {
        /*aloca*/
        *array_of_nodes = malloc(sizeof(Node *)*MAX_CHILDREN_NUMBER);
        /*inicializa*/
        for(i=0; i< MAX_CHILDREN_NUMBER; i++)
            (*array_of_nodes)[i] = NULL;
    }
    /*coloca o novo*/
    (*array_of_nodes)[cur_size] = n;
    return cur_size+1;
}


