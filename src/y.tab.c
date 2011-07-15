
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "pico.y"

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




/* Line 189 of yacc.c  */
#line 237 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 164 "pico.y"

  char* cadeia;
  struct _node * no;



/* Line 214 of yacc.c  */
#line 344 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 356 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   139

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  54
/* YYNRULES -- Number of states.  */
#define YYNSTATES  102

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,    45,    43,    35,    44,     2,    46,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,    33,
      48,    42,    47,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    11,    15,    19,    21,    25,
      27,    29,    31,    33,    35,    37,    39,    44,    48,    50,
      53,    57,    61,    63,    65,    70,    72,    76,    80,    84,
      88,    92,    96,    98,   100,   102,   104,   109,   111,   119,
     127,   129,   133,   135,   137,   141,   145,   149,   152,   156,
     158,   160,   162,   164,   166
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      50,     0,    -1,    51,    58,    -1,    58,    -1,    52,    33,
      -1,    51,    52,    33,    -1,    53,    34,    54,    -1,     3,
      -1,     3,    35,    53,    -1,    55,    -1,    56,    -1,     5,
      -1,     6,    -1,     7,    -1,     9,    -1,     8,    -1,    55,
      36,    57,    37,    -1,    29,    34,    29,    -1,    29,    -1,
      59,    33,    -1,    59,    33,    58,    -1,    60,    42,    62,
      -1,    64,    -1,     3,    -1,     3,    36,    61,    37,    -1,
      62,    -1,    62,    35,    61,    -1,    62,    43,    62,    -1,
      62,    44,    62,    -1,    62,    45,    62,    -1,    62,    46,
      62,    -1,    38,    62,    39,    -1,    29,    -1,    30,    -1,
      60,    -1,    63,    -1,     3,    38,    61,    39,    -1,    62,
      -1,    19,    38,    66,    39,    20,    58,    65,    -1,    22,
      38,    66,    39,    40,    58,    41,    -1,    28,    -1,    21,
      58,    28,    -1,    31,    -1,    32,    -1,    38,    66,    39,
      -1,    66,    16,    66,    -1,    66,    17,    66,    -1,    18,
      66,    -1,    62,    67,    62,    -1,    47,    -1,    48,    -1,
      13,    -1,    12,    -1,    14,    -1,    15,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   247,   247,   259,   264,   269,   276,   284,   285,   292,
     293,   296,   297,   298,   299,   300,   303,   320,   328,   338,
     348,   362,   382,   385,   392,   440,   441,   449,   471,   491,
     511,   531,   543,   548,   553,   554,   557,   566,   567,   619,
     652,   653,   665,   671,   677,   678,   696,   714,   720,   746,
     747,   748,   749,   750,   751
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDF", "CONST", "INT", "DOUBLE", "FLOAT",
  "REAL", "CHAR", "QUOTE", "DQUOTE", "LE", "GE", "EQ", "NE", "AND", "OR",
  "NOT", "IF", "THEN", "ELSE", "WHILE", "FOR", "NEXT", "REPEAT", "UNTIL",
  "CASE", "END", "INT_LIT", "F_LIT", "TRUE", "FALSE", "';'", "':'", "','",
  "'['", "']'", "'('", "')'", "'{'", "'}'", "'='", "'+'", "'-'", "'*'",
  "'/'", "'>'", "'<'", "$accept", "code", "declaracoes", "declaracao",
  "listadeclaracao", "tipo", "tipounico", "tipolista", "listadupla",
  "acoes", "comando", "lvalue", "listaexpr", "expr", "chamaproc",
  "enunciado", "fiminstcontrole", "expbool", "operbool", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,    59,    58,    44,    91,    93,    40,    41,
     123,   125,    61,    43,    45,    42,    47,    62,    60
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    53,    53,    54,
      54,    55,    55,    55,    55,    55,    56,    57,    57,    58,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    64,    64,    64,
      65,    65,    66,    66,    66,    66,    66,    66,    66,    67,
      67,    67,    67,    67,    67
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     3,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     1,     2,
       3,     3,     1,     1,     4,     1,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     4,     1,     7,     7,
       1,     3,     1,     1,     3,     3,     3,     2,     3,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    23,     0,     0,    32,    33,     0,     0,     0,     0,
       0,     3,     0,    34,    37,    35,    22,     0,     0,     0,
       0,     0,    23,    34,     0,     1,     0,     2,     4,     0,
      19,     0,     0,     0,     0,     0,     7,     8,     0,    25,
       0,     0,    42,    43,     0,     0,     0,     0,    31,     5,
      11,    12,    13,    15,    14,     6,     9,    10,    20,    21,
      27,    28,    29,    30,    24,     0,    36,    47,     0,     0,
      52,    51,    53,    54,    49,    50,     0,     0,     0,     0,
       0,     0,    26,    44,    48,    45,    46,     0,     0,    18,
       0,     0,     0,     0,    16,     0,    40,    38,    39,    17,
       0,    41
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    55,    56,    57,    90,    11,
      12,    23,    38,    14,    15,    16,    97,    46,    76
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -35
static const yytype_int8 yypact[] =
{
      24,    73,     3,     6,   -35,   -35,    72,    49,    24,    28,
      29,   -35,    34,    48,    12,   -35,   -35,    93,    72,    72,
      62,    62,    -7,   -35,    83,   -35,    43,   -35,   -35,     8,
     101,    72,    72,    72,    72,    72,    68,   -35,    60,   -34,
      66,    62,   -35,   -35,    62,    71,   -11,    35,   -35,   -35,
     -35,   -35,   -35,   -35,   -35,   -35,    70,   -35,   -35,    12,
       2,     2,   -35,   -35,   -35,    72,   -35,   -35,    25,    82,
     -35,   -35,   -35,   -35,   -35,   -35,    72,    62,    62,    92,
      84,    96,   -35,   -35,    12,   -35,    97,   101,   101,    98,
      99,    -3,    94,   104,   -35,   101,   -35,   -35,   -35,   -35,
     106,   -35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -35,   -35,   -35,   129,   121,   -35,   -35,   -35,   -35,    -6,
     -35,     0,   -15,     1,   -35,   -35,   -35,   -18,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -8
static const yytype_int8 yytable[] =
{
      13,    65,    27,    47,    40,    77,    78,    24,    13,    32,
      33,    34,    35,    50,    51,    52,    53,    54,    95,    39,
      39,    45,    45,    67,    58,    96,    69,     1,    79,    18,
      13,    19,    59,    60,    61,    62,    63,    70,    71,    72,
      73,    20,    45,     2,    21,    68,     3,    34,    35,    25,
      82,    77,    78,     4,     5,    32,    33,    34,    35,    85,
      86,    28,     6,    29,    48,    22,    39,    30,    32,    33,
      34,    35,    74,    75,    80,    22,    49,    84,    45,    45,
      41,    91,    92,    70,    71,    72,    73,    13,    13,   100,
      31,     4,     5,    42,    43,    13,    36,    64,    77,    78,
      44,     4,     5,    17,    22,    66,    81,    -7,    17,    18,
       6,    19,    87,    77,    32,    33,    34,    35,    74,    75,
       2,    83,    48,     3,    88,    89,    32,    33,    34,    35,
       4,     5,    93,    99,   101,    98,    94,    26,    37,     6
};

static const yytype_uint8 yycheck[] =
{
       0,    35,     8,    21,    19,    16,    17,     6,     8,    43,
      44,    45,    46,     5,     6,     7,     8,     9,    21,    18,
      19,    20,    21,    41,    30,    28,    44,     3,    39,    36,
      30,    38,    31,    32,    33,    34,    35,    12,    13,    14,
      15,    38,    41,    19,    38,    44,    22,    45,    46,     0,
      65,    16,    17,    29,    30,    43,    44,    45,    46,    77,
      78,    33,    38,    34,    39,     3,    65,    33,    43,    44,
      45,    46,    47,    48,    39,     3,    33,    76,    77,    78,
      18,    87,    88,    12,    13,    14,    15,    87,    88,    95,
      42,    29,    30,    31,    32,    95,     3,    37,    16,    17,
      38,    29,    30,    35,     3,    39,    36,    34,    35,    36,
      38,    38,    20,    16,    43,    44,    45,    46,    47,    48,
      19,    39,    39,    22,    40,    29,    43,    44,    45,    46,
      29,    30,    34,    29,    28,    41,    37,     8,    17,    38
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    19,    22,    29,    30,    38,    50,    51,    52,
      53,    58,    59,    60,    62,    63,    64,    35,    36,    38,
      38,    38,     3,    60,    62,     0,    52,    58,    33,    34,
      33,    42,    43,    44,    45,    46,     3,    53,    61,    62,
      61,    18,    31,    32,    38,    62,    66,    66,    39,    33,
       5,     6,     7,     8,     9,    54,    55,    56,    58,    62,
      62,    62,    62,    62,    37,    35,    39,    66,    62,    66,
      12,    13,    14,    15,    47,    48,    67,    16,    17,    39,
      39,    36,    61,    39,    62,    66,    66,    20,    40,    29,
      57,    58,    58,    34,    37,    21,    28,    65,    41,    29,
      58,    28
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 247 "pico.y"
    { Node **c; 
						  pack_nodes(&c, 0, (yyvsp[(1) - (2)].no));
						  pack_nodes(&c, 1, (yyvsp[(2) - (2)].no));

						  // Attribute synth
						  code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						  //cat_tac(&attr->code, &((expr_attr*)c[0]->attribute)->code);
						  cat_tac(&attr->code, &((expr_attr*)c[1]->attribute)->code);

						  (yyval.no) = create_node(0, program_node, "code", attr, 2, c);
						  syntax_tree = (yyval.no);
						}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 259 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no);
						  syntax_tree = (yyval.no);  
						}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 264 "pico.y"
    { Node **c;
										  pack_nodes(&c, 0, (yyvsp[(1) - (2)].no));
										  pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
										  (yyval.no) = create_node(0, decl_node, "declaracoes", NULL, 2, c);
										}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 269 "pico.y"
    { Node **c;
		   								  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
										  pack_nodes(&c, 1, (yyvsp[(2) - (3)].no));
										  (yyval.no) = create_node(0, decl_node, "declaracoes", NULL, 2, c);
										}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 276 "pico.y"
    { Node **c, *n;
									   insert_decl((yyvsp[(1) - (3)].no), (yyvsp[(3) - (3)].no));
									   print_table(symbol_table);
									   pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
									   pack_nodes(&c, 1, (yyvsp[(3) - (3)].no));
									   (yyval.no) = create_node(0, decl_node, "declaracao", NULL, 2, c);
									 }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 284 "pico.y"
    { (yyval.no) = create_leaf(0, idf_node, (yyvsp[(1) - (1)].cadeia), NULL); }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 285 "pico.y"
    { Node **c;
			   							   pack_nodes(&c, 0, create_leaf(0, idf_node, (yyvsp[(1) - (3)].cadeia), NULL));
										   pack_nodes(&c, 1, (yyvsp[(3) - (3)].no));
										   (yyval.no) = create_node(0, decl_list_node, "lista declaracao", NULL, 2, c);
										 }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 292 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 293 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 296 "pico.y"
    { (yyval.no) = create_leaf(0, int_node, "int", NULL); }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 297 "pico.y"
    { (yyval.no) = create_leaf(0, double_node, "double", NULL); }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 298 "pico.y"
    { (yyval.no) = create_leaf(0, float_node, "float", NULL); }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 299 "pico.y"
    { (yyval.no) = create_leaf(0, char_node, "char", NULL); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 300 "pico.y"
    { (yyval.no) = create_leaf(0, real_node, "real", NULL); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 303 "pico.y"
    { Node **c;
									   	 tipolista_attr *l = malloc(sizeof(tipolista_attr));
									   	 listadupla_attr *ld = (yyvsp[(3) - (4)].no)->attribute;
									   	 l->tipo = node_type((yyvsp[(1) - (4)].no)->type);
									   	 l->ld = ld;
									   	 l->size = node_size((yyvsp[(1) - (4)].no)->type) * ld->size;
										 pack_nodes(&c, 0, (yyvsp[(1) - (4)].no));
									   	 pack_nodes(&c, 1, (yyvsp[(3) - (4)].no));
									   	 (yyval.no) = create_node(0, tipolista_node, "tipo lista", l, 2, c);
									 }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 320 "pico.y"
    { Node **c;
								  listadupla_attr *l = malloc(sizeof(listadupla_attr));
								  l->first = atoi((yyvsp[(1) - (3)].cadeia));
								  l->size = atoi((yyvsp[(3) - (3)].cadeia)) - l->first + 1;
								  pack_nodes(&c, 0, create_leaf(0, intlit_node, (yyvsp[(1) - (3)].cadeia), NULL));
								  pack_nodes(&c, 1, create_leaf(0, intlit_node, (yyvsp[(3) - (3)].cadeia), NULL));
								  (yyval.no) = create_node(0, listadupla_node, "lista dupla", l, 2, c);
								}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 328 "pico.y"
    { Node **c;
		  						  listadupla_attr *l = malloc(sizeof(listadupla_attr));
								  l->first = 0;
								  l->size = atoi((yyvsp[(1) - (1)].cadeia)) + 1;
								  (yyval.no) = create_leaf(0, listadupla_node, (yyvsp[(1) - (1)].cadeia), l);
								}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 338 "pico.y"
    { Node **c;
						   pack_nodes(&c, 0, (yyvsp[(1) - (2)].no));
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));

						   // Attribute synth
						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						   cat_tac(&attr->code, &((code_attr*)c[0]->attribute)->code);
						   
						   (yyval.no) = create_node(0, acoes_node, "acoes", attr, 2, c);
						 }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 348 "pico.y"
    { Node **c;
						   pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
						   pack_nodes(&c, 1, create_leaf(0, semicolon_node, ";", NULL));
						   pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
						   
						   // Attribute synth
 						   code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
						   cat_tac(&attr->code, &((code_attr*)c[0]->attribute)->code);
						   cat_tac(&attr->code, &((code_attr*)c[2]->attribute)->code);
						   
						   (yyval.no) = create_node(0, acoes_node, "acoes", attr, 3, c);
						 }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 362 "pico.y"
    { Node **c;
						   pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
						   pack_nodes(&c, 1, create_leaf(0, attrib_node, ":=", NULL));
						   pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));

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

						   (yyval.no) = create_node(0, comando_node, "comando", attr, 3, c);
						 }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 382 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no);}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 385 "pico.y"
    { entry_t *ref = lookup(symbol_table, (yyvsp[(1) - (1)].cadeia));
								if (ref == NULL) return picoerror(UNDEFINED_SYM_ERROR);
								expr_attr *attr = (expr_attr*)malloc(sizeof(expr_attr));
					  			attr->local = malloc(sizeof(char) * 8);
					  			sprintf(attr->local, "%03d(SP)", ref->desloc);
								(yyval.no) = create_leaf(0, idf_node, (yyvsp[(1) - (1)].cadeia), attr); 
							  }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 392 "pico.y"
    { Node **c;
							    pack_nodes(&c, 0, create_leaf(0, idf_node, (yyvsp[(1) - (4)].cadeia), NULL));
							    pack_nodes(&c, 1, create_leaf(0, opencol_node, "[", NULL));
							    pack_nodes(&c, 2, (yyvsp[(3) - (4)].no));
							    pack_nodes(&c, 3, create_leaf(0, closecol_node, "]", NULL));

							    entry_t *ref = lookup(symbol_table, (yyvsp[(1) - (4)].cadeia));
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
								(yyval.no) = create_node(0, idf_node, "lvalue", attr, 4, c);
							  }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 440 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 441 "pico.y"
    { Node **c;
	   						  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
							  pack_nodes(&c, 1, create_leaf(0, comma_node, ",", NULL));
							  pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
							  (yyval.no) = create_node(0, listaexpr_node, "lista expr", NULL, 3, c);
							}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 449 "pico.y"
    { // Pack nodes
					  Node **c;
					  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
					  pack_nodes(&c, 1, create_leaf(0, plus_node, "+", NULL));
					  pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
					  
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
					  (yyval.no) = create_node(0, expr_node, "expr", attr, 3, c);
					}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 471 "pico.y"
    { Node **c;
					  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
					  pack_nodes(&c, 1, create_leaf(0, minus_node, "-", NULL));
					  pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
					  
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
					  
					  (yyval.no) = create_node(0, expr_node, "expr", attr, 3, c);
					}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 491 "pico.y"
    { Node **c;
					  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
					  pack_nodes(&c, 1, create_leaf(0, mult_node, "*", NULL));
					  pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
					  
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
					  
					  (yyval.no) = create_node(0, expr_node, "expr", attr, 3, c);
					}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 511 "pico.y"
    { Node **c;
					  pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
					  pack_nodes(&c, 1, create_leaf(0, div_node, "/", NULL));
					  pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
					  
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
					  
					  (yyval.no) = create_node(0, expr_node, "expr", attr, 3, c);
					}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 531 "pico.y"
    { Node **c;
					  pack_nodes(&c, 0, create_leaf(0, openpar_node, "(", NULL));
					  pack_nodes(&c, 1, (yyvsp[(2) - (3)].no));
					  pack_nodes(&c, 2, create_leaf(0, closepar_node, ")", NULL));
					  
					  // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));;
					  attr->local = ((expr_attr*)c[1]->attribute)->local;
					  cat_tac(&attr->code, &((expr_attr*)c[1]->attribute)->code);
					  
					  (yyval.no) = create_node(0, tipolista_node, "expr", attr, 3, c);
					}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 543 "pico.y"
    { // Attribute synth
					  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));
					  attr->local = (yyvsp[(1) - (1)].cadeia);
					  (yyval.no) = create_leaf(0, intlit_node, (yyvsp[(1) - (1)].cadeia), attr);
    				}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 548 "pico.y"
    { // Attribute synth
    				  expr_attr *attr = (expr_attr*) malloc(sizeof(expr_attr));
			    	  attr->local = (yyvsp[(1) - (1)].cadeia);
    				  (yyval.no) = create_leaf(0, floatlit_node, (yyvsp[(1) - (1)].cadeia), attr);
    				}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 553 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 554 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 557 "pico.y"
    { Node **c;
								   pack_nodes(&c, 0, create_leaf(0, idf_node, (yyvsp[(2) - (4)].cadeia), NULL));
					  			   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
								   pack_nodes(&c, 2, (yyvsp[(3) - (4)].no));
					  			   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
								   create_node(0, proc_node, "chamaproc", NULL, 4, c);
								 }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 566 "pico.y"
    { (yyval.no) = (yyvsp[(1) - (1)].no); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 567 "pico.y"
    {
															int hasElse = ((yyvsp[(7) - (7)].no)->type == fimcontrole_node);
															printf("hasElse? %i\n", hasElse);
															
															Node **c;
															pack_nodes(&c, 0, create_leaf(0, if_node, "if", NULL));
															pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
															pack_nodes(&c, 2, (yyvsp[(3) - (7)].no)); //B
															pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
															pack_nodes(&c, 4, create_leaf(0, then_node, "then", NULL));
															pack_nodes(&c, 5, (yyvsp[(6) - (7)].no)); //S1
															pack_nodes(&c, 6, (yyvsp[(7) - (7)].no)); //S2


															// Attribute synth	
															code_attr *attr = (code_attr*) malloc(sizeof(code_attr));

															//B.t = novo_rot
															((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->t = novo_rotulo(); //should be updating the '#' in "IF xxx GOTO LABEL#", but is not working (why?).
															
															// calcula S.next...
															attr->next = novo_rotulo();//**********************TODO**********************
															
															if(hasElse)
																((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->f = novo_rotulo();
															else
																((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->f = attr->next;

															//S1.next = S.next
															((code_attr*)(yyvsp[(6) - (7)].no)->attribute)->next = attr->next;
															
															//S1 and S2 attribute synth
															if(hasElse) {
																((code_attr*)(yyvsp[(7) - (7)].no)->attribute)->next = attr->next; //S2.next = S.next
																
																cat_tac(&attr->code, &((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->code); //B.code
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)((yyvsp[(3) - (7)].no)->attribute))->t, "", "", "") ); //geracod(B.t)
																cat_tac(&attr->code, &((code_attr*)(yyvsp[(6) - (7)].no)->attribute)->code); //S1.code														   
																append_inst_tac(&attr->code, create_inst_tac( "GOTO",attr->next,"","") ); //geracod('goto' S.next)
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)((yyvsp[(3) - (7)].no)->attribute))->f, "", "", "") ); //geracod(B.f)
																cat_tac(&attr->code, &((code_attr*)(yyvsp[(7) - (7)].no)->attribute)->code); //S2.code												
															}
															else {
																cat_tac(&attr->code, &((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->code); //B.code
																append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)((yyvsp[(3) - (7)].no)->attribute))->t, "", "", "") ); //geracod(B.t)
																cat_tac(&attr->code, &((code_attr*)(yyvsp[(6) - (7)].no)->attribute)->code); //S1.code														   
															}
															append_inst_tac(&attr->code, create_inst_tac( attr->next,"","","") ); //geracod(S.next)
															

															(yyval.no) = create_node(0, enunciado_node, "enunciado", attr, 7, c);
														 }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 619 "pico.y"
    { Node **c;
														   pack_nodes(&c, 0, create_leaf(0, while_node, "while", NULL));
														   pack_nodes(&c, 1, create_leaf(0, openpar_node, "(", NULL));
														   pack_nodes(&c, 2, (yyvsp[(3) - (7)].no)); //B
														   pack_nodes(&c, 3, create_leaf(0, closepar_node, ")", NULL));
														   pack_nodes(&c, 4, create_leaf(0, openbra_node, "{", NULL));
														   pack_nodes(&c, 5, (yyvsp[(6) - (7)].no)); //S1
														   pack_nodes(&c, 6, create_leaf(0, closebra_node, "}", NULL));
														   
														   // Attribute synth	
															while_attr *attr = (while_attr*) malloc(sizeof(while_attr));

															// calcula S.next
															attr->next = novo_rotulo(); //**********************TODO**********************

															((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->f = attr->next;
															((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->t = novo_rotulo();

															attr->begin = novo_rotulo(); //S.begin = novo_rot
															((while_attr*)(yyvsp[(6) - (7)].no)->attribute)->next = attr->begin; //S1.next = S.begin

															append_inst_tac(&attr->code, create_inst_tac( attr->begin,"","","") ); //geracod(S.begin)
															cat_tac(&attr->code, &((tipobool_attr*)(yyvsp[(3) - (7)].no)->attribute)->code); //B.code	
															append_inst_tac(&attr->code, create_inst_tac( ((tipobool_attr*)((yyvsp[(3) - (7)].no)->attribute))->t,"","","") ); //geracod(B.t)
															cat_tac(&attr->code, &((code_attr*)(yyvsp[(6) - (7)].no)->attribute)->code); //S1.code	
															append_inst_tac(&attr->code, create_inst_tac( "GOTO",attr->begin,"","") ); //geracod('goto' S.begin)
															append_inst_tac(&attr->code, create_inst_tac( attr->next,"","","") ); //geracod(S.begin)

														   
														   (yyval.no) = create_node(0, enunciado_node, "enunciado", attr, 7, c);
														 }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 652 "pico.y"
    { (yyval.no) = create_leaf(0, end_node, "end", NULL); }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 653 "pico.y"
    { Node **c;
								  pack_nodes(&c, 0, create_leaf(0, else_node, "else", NULL));
								  pack_nodes(&c, 1, (yyvsp[(2) - (3)].no));
								  pack_nodes(&c, 2, create_leaf(0, end_node, "end", NULL));
								  
								  code_attr *attr = (code_attr*) malloc(sizeof(code_attr));
									cat_tac(&attr->code, &((code_attr*)(yyvsp[(2) - (3)].no)->attribute)->code);
								  
								  (yyval.no) = create_node(0, fimcontrole_node, "fiminstcontrole", attr, 3, c);
								}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 665 "pico.y"
    { tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								attr->t = novo_rotulo();
								attr->f = novo_rotulo();
								append_inst_tac(&attr->code, create_inst_tac("GOTO",attr->t,"",""));
								(yyval.no) = create_leaf(0, true_node, "true", attr);
							  }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 671 "pico.y"
    { tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								attr->t = novo_rotulo();
								attr->f = novo_rotulo();
								append_inst_tac(&attr->code, create_inst_tac("GOTO",attr->f,"",""));
								(yyval.no) = create_leaf(0, false_node, "false", attr);
							  }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 677 "pico.y"
    { (yyval.no) = (yyvsp[(2) - (3)].no); }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 678 "pico.y"
    { Node **c;
	   							pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
							    pack_nodes(&c, 1, create_leaf(0, and_node, "and", NULL));
								pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
								
								// Attr synth
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								tipobool_attr *b1 = ((tipobool_attr *)(yyvsp[(1) - (3)].no)->attribute);
         						tipobool_attr *b2 = ((tipobool_attr *)(yyvsp[(3) - (3)].no)->attribute);

								attr->f = b1->f;
								attr->t = b2->t;

								cat_tac(&attr->code, &b1->code);
								append_inst_tac(&attr->code,create_inst_tac(b1->t, "", "", ""));
								cat_tac(&attr->code, &b2->code);
								(yyval.no) = create_node(0, and_node, "expbool", attr, 3, c);
							  }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 696 "pico.y"
    { Node **c;
	   							pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
							    pack_nodes(&c, 1, create_leaf(0, and_node, "or", NULL));
								pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
								
								// Attr synth
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								tipobool_attr *b1 = ((tipobool_attr *)(yyvsp[(1) - (3)].no)->attribute);
         						tipobool_attr *b2 = ((tipobool_attr *)(yyvsp[(3) - (3)].no)->attribute);

								attr->f = b1->f;
								attr->t = b2->t;

								cat_tac(&attr->code, &b1->code);
								append_inst_tac(&attr->code,create_inst_tac(b1->f, "", "", ""));
								cat_tac(&attr->code, &b2->code);
								(yyval.no) = create_node(0, or_node, "expbool", attr, 3, c);
							  }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 714 "pico.y"
    { Node **c; 
							    pack_nodes(&c, 0, create_leaf(0, not_node, "not", NULL));
	   						    pack_nodes(&c, 1, (yyvsp[(2) - (2)].no)); 
	   						    // inverter os labels... (como?)
								(yyval.no) = create_node(0, expbool_node, "expbool", NULL, 2, c); 
							  }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 720 "pico.y"
    { Node **c;
	   							pack_nodes(&c, 0, (yyvsp[(1) - (3)].no));
							    pack_nodes(&c, 1, create_leaf(0, op_node, (yyvsp[(2) - (3)].no)->lexeme, NULL));
								pack_nodes(&c, 2, (yyvsp[(3) - (3)].no));
								
								tipobool_attr *attr = malloc(sizeof(tipobool_attr));
								expr_attr *e1 = ((expr_attr *)(yyvsp[(1) - (3)].no)->attribute);
								expr_attr *e2 = ((expr_attr *)(yyvsp[(3) - (3)].no)->attribute);

								attr->t = novo_rotulo();
								attr->f = novo_rotulo();

								cat_tac(&attr->code, &e1->code);
								cat_tac(&attr->code, &e2->code);
								append_inst_tac(&attr->code,create_inst_tac(attr->t, e1->local, ">", e2->local));
								append_inst_tac(&attr->code,create_inst_tac("GOTO", attr->f, "", ""));
								(yyval.no) = create_node(0, op_node, "expbool", attr, 3, c);
	   						  }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 746 "pico.y"
    { (yyval.no) = create_leaf(0, sup_node, ">", NULL); }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 747 "pico.y"
    { (yyval.no) = create_leaf(0, inf_node, "<", NULL); }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 748 "pico.y"
    { (yyval.no) = create_leaf(0, sup_eq_node, ">=", NULL); }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 749 "pico.y"
    { (yyval.no) = create_leaf(0, inf_eq_node, "<=", NULL); }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 750 "pico.y"
    { (yyval.no) = create_leaf(0, eq_node, "==", NULL); }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 751 "pico.y"
    { (yyval.no) = create_leaf(0, neq_node, "!=", NULL); }
    break;



/* Line 1455 of yacc.c  */
#line 2418 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 753 "pico.y"


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
