
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDF = 258,
     CONST = 259,
     INT = 260,
     DOUBLE = 261,
     FLOAT = 262,
     REAL = 263,
     CHAR = 264,
     QUOTE = 265,
     DQUOTE = 266,
     LE = 267,
     GE = 268,
     EQ = 269,
     NE = 270,
     AND = 271,
     OR = 272,
     NOT = 273,
     IF = 274,
     THEN = 275,
     ELSE = 276,
     WHILE = 277,
     FOR = 278,
     NEXT = 279,
     REPEAT = 280,
     UNTIL = 281,
     CASE = 282,
     END = 283,
     INT_LIT = 284,
     F_LIT = 285,
     TRUE = 286,
     FALSE = 287
   };
#endif
/* Tokens.  */
#define IDF 258
#define CONST 259
#define INT 260
#define DOUBLE 261
#define FLOAT 262
#define REAL 263
#define CHAR 264
#define QUOTE 265
#define DQUOTE 266
#define LE 267
#define GE 268
#define EQ 269
#define NE 270
#define AND 271
#define OR 272
#define NOT 273
#define IF 274
#define THEN 275
#define ELSE 276
#define WHILE 277
#define FOR 278
#define NEXT 279
#define REPEAT 280
#define UNTIL 281
#define CASE 282
#define END 283
#define INT_LIT 284
#define F_LIT 285
#define TRUE 286
#define FALSE 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 66 "pico.y"

  char* cadeia;
  struct _node * no;



/* Line 1676 of yacc.c  */
#line 123 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


