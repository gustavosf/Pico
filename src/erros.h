/** @file erros.h
 * \version 1
 * \author Daniel Beck
 * \author Francieli Zanon
 * \author Joseane Barrios
 * \brief definicao dos erros possi�veis no programa*/

#ifndef _ERROS_H_
#define _ERROS_H_

#define FAILED -1  /*código do retorno de erro de função*/

/*tipos de erros*/
#define NARG  1  /*"NULL Argument" argumento passado era NULL (e não podia)*/
#define OLARG 2  /*"Out of Limit Argument" argumento inteiro passada estava fora do limite permitido*/   

/*função que seta a variável do último erro para o tipo informado e retorna o
 * código de erro na função*/
//int notify_error(int type);
/*função que retorna o tipo do último erro ocorrido*/
//int get_error();
#endif





