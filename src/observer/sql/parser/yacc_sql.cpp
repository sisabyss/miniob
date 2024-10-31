/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <optional>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 126 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_DROP = 6,                       /* DROP  */
  YYSYMBOL_GROUP = 7,                      /* GROUP  */
  YYSYMBOL_TABLE = 8,                      /* TABLE  */
  YYSYMBOL_TABLES = 9,                     /* TABLES  */
  YYSYMBOL_INDEX = 10,                     /* INDEX  */
  YYSYMBOL_CALC = 11,                      /* CALC  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_DESC = 13,                      /* DESC  */
  YYSYMBOL_SHOW = 14,                      /* SHOW  */
  YYSYMBOL_SYNC = 15,                      /* SYNC  */
  YYSYMBOL_INSERT = 16,                    /* INSERT  */
  YYSYMBOL_DELETE = 17,                    /* DELETE  */
  YYSYMBOL_UPDATE = 18,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 19,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 20,                    /* RBRACE  */
  YYSYMBOL_COMMA = 21,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 22,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 23,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 24,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 25,                     /* INT_T  */
  YYSYMBOL_STRING_T = 26,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 27,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 28,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 29,                    /* TEXT_T  */
  YYSYMBOL_HELP = 30,                      /* HELP  */
  YYSYMBOL_EXIT = 31,                      /* EXIT  */
  YYSYMBOL_DOT = 32,                       /* DOT  */
  YYSYMBOL_INTO = 33,                      /* INTO  */
  YYSYMBOL_VALUES = 34,                    /* VALUES  */
  YYSYMBOL_FROM = 35,                      /* FROM  */
  YYSYMBOL_WHERE = 36,                     /* WHERE  */
  YYSYMBOL_AND = 37,                       /* AND  */
  YYSYMBOL_SET = 38,                       /* SET  */
  YYSYMBOL_ON = 39,                        /* ON  */
  YYSYMBOL_LOAD = 40,                      /* LOAD  */
  YYSYMBOL_DATA = 41,                      /* DATA  */
  YYSYMBOL_INFILE = 42,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 43,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 44,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 45,                    /* FORMAT  */
  YYSYMBOL_EQ = 46,                        /* EQ  */
  YYSYMBOL_LT = 47,                        /* LT  */
  YYSYMBOL_GT = 48,                        /* GT  */
  YYSYMBOL_LE = 49,                        /* LE  */
  YYSYMBOL_GE = 50,                        /* GE  */
  YYSYMBOL_NE = 51,                        /* NE  */
  YYSYMBOL_MAX = 52,                       /* MAX  */
  YYSYMBOL_MIN = 53,                       /* MIN  */
  YYSYMBOL_SUM = 54,                       /* SUM  */
  YYSYMBOL_AVG = 55,                       /* AVG  */
  YYSYMBOL_COUNT = 56,                     /* COUNT  */
  YYSYMBOL_INNER = 57,                     /* INNER  */
  YYSYMBOL_JOIN = 58,                      /* JOIN  */
  YYSYMBOL_UNIQUE = 59,                    /* UNIQUE  */
  YYSYMBOL_IS_SYM = 60,                    /* IS_SYM  */
  YYSYMBOL_NOT = 61,                       /* NOT  */
  YYSYMBOL_LIKE = 62,                      /* LIKE  */
  YYSYMBOL_NULL_SYM = 63,                  /* NULL_SYM  */
  YYSYMBOL_NULLABLE_SYM = 64,              /* NULLABLE_SYM  */
  YYSYMBOL_ORDER = 65,                     /* ORDER  */
  YYSYMBOL_ASC = 66,                       /* ASC  */
  YYSYMBOL_NUMBER = 67,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 68,                     /* FLOAT  */
  YYSYMBOL_ID = 69,                        /* ID  */
  YYSYMBOL_SSS = 70,                       /* SSS  */
  YYSYMBOL_71_ = 71,                       /* '+'  */
  YYSYMBOL_72_ = 72,                       /* '-'  */
  YYSYMBOL_73_ = 73,                       /* '*'  */
  YYSYMBOL_74_ = 74,                       /* '/'  */
  YYSYMBOL_UMINUS = 75,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 76,                  /* $accept  */
  YYSYMBOL_commands = 77,                  /* commands  */
  YYSYMBOL_command_wrapper = 78,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 79,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 80,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 81,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 82,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 83,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 84,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 85,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 86,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 87,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 88,         /* create_index_stmt  */
  YYSYMBOL_opt_unique = 89,                /* opt_unique  */
  YYSYMBOL_ID_list = 90,                   /* ID_list  */
  YYSYMBOL_drop_index_stmt = 91,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 92,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 93,             /* attr_def_list  */
  YYSYMBOL_attr_def = 94,                  /* attr_def  */
  YYSYMBOL_opt_null = 95,                  /* opt_null  */
  YYSYMBOL_number = 96,                    /* number  */
  YYSYMBOL_type = 97,                      /* type  */
  YYSYMBOL_insert_stmt = 98,               /* insert_stmt  */
  YYSYMBOL_value_list = 99,                /* value_list  */
  YYSYMBOL_value = 100,                    /* value  */
  YYSYMBOL_storage_format = 101,           /* storage_format  */
  YYSYMBOL_delete_stmt = 102,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 103,              /* update_stmt  */
  YYSYMBOL_select_stmt = 104,              /* select_stmt  */
  YYSYMBOL_opt_order_by = 105,             /* opt_order_by  */
  YYSYMBOL_order_by_list = 106,            /* order_by_list  */
  YYSYMBOL_order_by = 107,                 /* order_by  */
  YYSYMBOL_calc_stmt = 108,                /* calc_stmt  */
  YYSYMBOL_expression_list = 109,          /* expression_list  */
  YYSYMBOL_expression = 110,               /* expression  */
  YYSYMBOL_rel_attr = 111,                 /* rel_attr  */
  YYSYMBOL_relation = 112,                 /* relation  */
  YYSYMBOL_table_ref_list = 113,           /* table_ref_list  */
  YYSYMBOL_comma_ref_list = 114,           /* comma_ref_list  */
  YYSYMBOL_join_ref_list = 115,            /* join_ref_list  */
  YYSYMBOL_where = 116,                    /* where  */
  YYSYMBOL_condition_list = 117,           /* condition_list  */
  YYSYMBOL_condition = 118,                /* condition  */
  YYSYMBOL_is_null_comp = 119,             /* is_null_comp  */
  YYSYMBOL_comp_op = 120,                  /* comp_op  */
  YYSYMBOL_group_by = 121,                 /* group_by  */
  YYSYMBOL_load_data_stmt = 122,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 123,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 124,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 125             /* opt_semicolon  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  230

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   326


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    73,    71,     2,    72,     2,    74,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    75
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   219,   219,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   250,   256,   261,   267,   273,   279,   285,
     292,   298,   306,   321,   324,   329,   335,   348,   358,   382,
     385,   398,   407,   431,   434,   437,   440,   445,   448,   449,
     450,   451,   452,   455,   472,   475,   486,   499,   504,   508,
     512,   521,   524,   531,   543,   559,   591,   594,   601,   607,
     620,   632,   644,   659,   668,   673,   684,   688,   691,   694,
     697,   700,   704,   709,   715,   719,   728,   737,   746,   755,
     761,   766,   776,   781,   784,   789,   794,   806,   820,   841,
     844,   850,   853,   858,   865,   872,   881,   884,   890,   891,
     892,   893,   894,   895,   896,   897,   903,   906,   912,   925,
     933,   943,   944
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "DROP", "GROUP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT",
  "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "FLOAT_T", "DATE_T", "TEXT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE", "NE", "MAX", "MIN",
  "SUM", "AVG", "COUNT", "INNER", "JOIN", "UNIQUE", "IS_SYM", "NOT",
  "LIKE", "NULL_SYM", "NULLABLE_SYM", "ORDER", "ASC", "NUMBER", "FLOAT",
  "ID", "SSS", "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept", "commands",
  "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt",
  "commit_stmt", "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "opt_unique", "ID_list",
  "drop_index_stmt", "create_table_stmt", "attr_def_list", "attr_def",
  "opt_null", "number", "type", "insert_stmt", "value_list", "value",
  "storage_format", "delete_stmt", "update_stmt", "select_stmt",
  "opt_order_by", "order_by_list", "order_by", "calc_stmt",
  "expression_list", "expression", "rel_attr", "relation",
  "table_ref_list", "comma_ref_list", "join_ref_list", "where",
  "condition_list", "condition", "is_null_comp", "comp_op", "group_by",
  "load_data_stmt", "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     128,    23,    20,    25,    25,   -64,     0,  -153,   -21,   -16,
     -47,  -153,  -153,  -153,  -153,  -153,   -40,    10,   128,    86,
      87,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,    22,  -153,    89,    27,    40,    25,    94,    96,    97,
     102,   104,  -153,  -153,  -153,    92,  -153,    25,  -153,  -153,
    -153,    64,  -153,    91,  -153,  -153,    60,    61,    93,   101,
     106,  -153,  -153,  -153,  -153,   130,    84,  -153,   115,   -14,
      25,    25,    25,    25,    25,    88,  -153,  -153,    25,    25,
      25,    25,    25,    98,   121,   120,   100,   -52,    90,   109,
     123,   110,  -153,    -3,     1,    29,    33,    46,  -153,  -153,
     -31,   -31,  -153,  -153,  -153,    -7,   120,  -153,   108,   151,
      25,  -153,   136,   -52,  -153,   150,    -2,   163,   116,  -153,
    -153,  -153,  -153,  -153,  -153,    98,   131,   183,   133,   -52,
     126,  -153,   155,   -52,  -153,   185,  -153,  -153,  -153,  -153,
    -153,    -9,   109,   174,   176,  -153,    98,   192,   137,    98,
     180,  -153,  -153,  -153,  -153,  -153,  -153,    51,   141,  -153,
    -153,    25,    25,   120,   135,   138,   143,  -153,  -153,  -153,
     163,   164,   140,   168,    25,   206,  -153,   172,   -52,   193,
     149,  -153,  -153,   -39,  -153,  -153,  -153,  -153,   194,  -153,
    -153,   170,  -153,   195,   197,    25,  -153,    25,    25,   180,
    -153,  -153,    47,   173,   140,  -153,  -153,  -153,   199,   -10,
    -153,  -153,  -153,   152,  -153,    25,  -153,  -153,  -153,  -153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    33,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     121,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    90,    60,     0,    84,    82,
      73,    74,    83,     0,    31,    30,     0,     0,     0,     0,
       0,   119,     1,   122,     2,     0,     0,    29,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    76,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,    81,     0,     0,     0,     0,     0,    91,    75,
      77,    78,    79,    80,    92,    95,    99,    93,    94,     0,
     101,    63,     0,     0,   120,     0,     0,    39,     0,    37,
      85,    86,    87,    88,    89,     0,     0,   116,     0,     0,
       0,   100,   102,     0,    56,     0,    48,    49,    50,    51,
      52,    43,     0,     0,     0,    96,     0,     0,    66,     0,
      54,   108,   109,   110,   111,   112,   113,     0,     0,   114,
     105,     0,   101,    99,     0,     0,     0,    45,    44,    42,
      39,    61,     0,     0,     0,     0,    65,     0,     0,     0,
       0,   106,   115,   104,   103,    64,   118,    47,     0,    46,
      40,     0,    38,    35,     0,   101,   117,     0,   101,    54,
      53,   107,    43,     0,     0,    32,    97,    67,    68,    70,
      98,    55,    41,     0,    36,     0,    72,    71,    62,    69
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,   200,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,     8,  -153,  -153,    43,    72,    13,
    -153,  -153,  -153,    17,   -56,  -153,  -153,  -153,  -153,  -153,
       2,  -153,  -153,    -4,   -44,  -153,  -152,    95,  -153,  -153,
    -108,   -83,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    43,   204,    31,    32,   153,   127,   179,
     198,   151,    33,   189,    59,   202,    34,    35,    36,   186,
     217,   218,    37,    60,    61,    62,   115,   116,   117,   118,
     121,   141,   142,   170,   171,   158,    38,    39,    40,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      63,    86,    79,   226,   183,    64,   102,   187,   137,    65,
     175,    52,    66,    87,   135,    53,    54,   130,    56,    67,
     123,   131,    68,   146,   147,   148,   149,   150,    44,    69,
      45,    41,    89,    90,    91,    92,   103,   104,   105,   106,
     107,   124,    91,    92,    46,   110,   111,   112,   113,   132,
     136,    70,   176,   133,   177,   178,   227,    89,    90,    91,
      92,    89,    90,    91,    92,   195,   134,   144,    89,    90,
      91,    92,    89,    90,    91,    92,   140,    47,    48,    49,
      50,    51,    42,   160,   109,    88,    72,   173,    52,   194,
      73,    75,    53,    54,    55,    56,    77,    57,    58,    76,
      89,    90,    91,    92,    89,    90,    91,    92,   176,    78,
     177,   178,   190,    80,   191,    81,    82,    89,    90,    91,
      92,    83,   216,    84,    85,   220,    93,   193,   140,    94,
      95,    96,   209,     1,     2,    89,    90,    91,    92,     3,
       4,     5,     6,     7,     8,     9,    10,    97,    98,    99,
      11,    12,    13,   100,   101,   119,   120,   108,    14,    15,
     125,   140,   128,   219,   140,   138,    16,   114,    17,   122,
     139,    18,   161,   162,   163,   164,   165,   166,   126,   129,
     206,   219,   143,   145,   152,   154,   167,   168,   169,   156,
     157,   159,   172,   174,   181,   182,   184,    89,    90,    91,
      92,   188,   185,   192,   196,   197,   199,   205,   201,   203,
     207,   208,   211,   210,   212,   213,   214,   215,    71,   223,
     225,   228,   224,   200,   180,   222,   221,   229,     0,     0,
     155
};

static const yytype_int16 yycheck[] =
{
       4,    57,    46,    13,   156,    69,    20,   159,   116,     9,
      19,    63,    33,    57,    21,    67,    68,    20,    70,    35,
      72,    20,    69,    25,    26,    27,    28,    29,     8,    69,
      10,     8,    71,    72,    73,    74,    80,    81,    82,    83,
      84,    97,    73,    74,    19,    89,    90,    91,    92,    20,
      57,    41,    61,    20,    63,    64,    66,    71,    72,    73,
      74,    71,    72,    73,    74,   173,    20,   123,    71,    72,
      73,    74,    71,    72,    73,    74,   120,    52,    53,    54,
      55,    56,    59,   139,    88,    21,     0,   143,    63,   172,
       3,    69,    67,    68,    69,    70,    69,    72,    73,    10,
      71,    72,    73,    74,    71,    72,    73,    74,    61,    69,
      63,    64,    61,    19,    63,    19,    19,    71,    72,    73,
      74,    19,   205,    19,    32,   208,    35,   171,   172,    69,
      69,    38,   188,     5,     6,    71,    72,    73,    74,    11,
      12,    13,    14,    15,    16,    17,    18,    46,    42,    19,
      22,    23,    24,    69,    39,    34,    36,    69,    30,    31,
      70,   205,    39,   207,   208,    57,    38,    69,    40,    69,
      19,    43,    46,    47,    48,    49,    50,    51,    69,    69,
     184,   225,    46,    33,    21,    69,    60,    61,    62,    58,
       7,    58,    37,     8,    20,    19,     4,    71,    72,    73,
      74,    21,    65,    62,    69,    67,    63,    39,    44,    69,
       4,    39,    63,    20,    20,    45,    21,    20,    18,    46,
      21,    69,   214,   180,   152,   212,   209,   225,    -1,    -1,
     135
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,     6,    11,    12,    13,    14,    15,    16,    17,
      18,    22,    23,    24,    30,    31,    38,    40,    43,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    91,    92,    98,   102,   103,   104,   108,   122,   123,
     124,     8,    59,    89,     8,    10,    19,    52,    53,    54,
      55,    56,    63,    67,    68,    69,    70,    72,    73,   100,
     109,   110,   111,   109,    69,     9,    33,    35,    69,    69,
      41,    78,     0,     3,   125,    69,    10,    69,    69,   110,
      19,    19,    19,    19,    19,    32,   100,   110,    21,    71,
      72,    73,    74,    35,    69,    69,    38,    46,    42,    19,
      69,    39,    20,   110,   110,   110,   110,   110,    69,   109,
     110,   110,   110,   110,    69,   112,   113,   114,   115,    34,
      36,   116,    69,    72,   100,    70,    69,    94,    39,    69,
      20,    20,    20,    20,    20,    21,    57,   116,    57,    19,
     110,   117,   118,    46,   100,    33,    25,    26,    27,    28,
      29,    97,    21,    93,    69,   113,    58,     7,   121,    58,
     100,    46,    47,    48,    49,    50,    51,    60,    61,    62,
     119,   120,    37,   100,     8,    19,    61,    63,    64,    95,
      94,    20,    19,   112,     4,    65,   105,   112,    21,    99,
      61,    63,    62,   110,   117,   116,    69,    67,    96,    63,
      93,    44,   101,    69,    90,    39,   109,     4,    39,   100,
      20,    63,    20,    45,    21,    20,   117,   106,   107,   110,
     117,    99,    95,    46,    90,    21,    13,    66,    69,   106
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    76,    77,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    89,    90,    90,    91,    92,    93,
      93,    94,    94,    95,    95,    95,    95,    96,    97,    97,
      97,    97,    97,    98,    99,    99,   100,   100,   100,   100,
     100,   101,   101,   102,   103,   104,   105,   105,   106,   106,
     107,   107,   107,   108,   109,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     111,   111,   112,   113,   113,   114,   114,   115,   115,   116,
     116,   117,   117,   117,   118,   118,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   121,   121,   122,   123,
     124,   125,   125
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,     0,     1,     1,     3,     5,     8,     0,
       3,     6,     3,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     8,     0,     3,     2,     1,     1,     1,
       1,     0,     4,     4,     7,     7,     0,     3,     1,     3,
       1,     2,     2,     2,     1,     3,     2,     3,     3,     3,
       3,     3,     1,     1,     1,     4,     4,     4,     4,     4,
       1,     3,     1,     1,     1,     1,     3,     6,     6,     0,
       2,     0,     1,     3,     3,     2,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     3,     7,     2,
       4,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 220 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1810 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 250 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1819 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 256 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1827 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 261 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1835 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 267 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1843 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 273 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1851 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 279 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1859 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 285 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1869 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 292 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1877 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 298 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1887 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE opt_unique INDEX ID ON ID LBRACE ID_list RBRACE  */
#line 307 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.unique = (yyvsp[-7].bools);
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);
      create_index.attribute_names.swap(*(yyvsp[-1].id_list));
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      delete (yyvsp[-1].id_list);
    }
#line 1903 "yacc_sql.cpp"
    break;

  case 33: /* opt_unique: %empty  */
#line 321 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 1911 "yacc_sql.cpp"
    break;

  case 34: /* opt_unique: UNIQUE  */
#line 324 "yacc_sql.y"
             {
      (yyval.bools) = true;
    }
#line 1919 "yacc_sql.cpp"
    break;

  case 35: /* ID_list: ID  */
#line 330 "yacc_sql.y"
    {
      (yyval.id_list) = new std::vector<std::string>;
      (yyval.id_list)->emplace_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 1929 "yacc_sql.cpp"
    break;

  case 36: /* ID_list: ID COMMA ID_list  */
#line 336 "yacc_sql.y"
    {
      if ((yyvsp[0].id_list) != nullptr) {
        (yyval.id_list) = (yyvsp[0].id_list);
      } else {
        (yyval.id_list) = new std::vector<std::string>;
      }
      (yyval.id_list)->emplace((yyval.id_list)->begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 1943 "yacc_sql.cpp"
    break;

  case 37: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 349 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 1955 "yacc_sql.cpp"
    break;

  case 38: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 359 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 1980 "yacc_sql.cpp"
    break;

  case 39: /* attr_def_list: %empty  */
#line 382 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 1988 "yacc_sql.cpp"
    break;

  case 40: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 386 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2002 "yacc_sql.cpp"
    break;

  case 41: /* attr_def: ID type LBRACE number RBRACE opt_null  */
#line 399 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-5].string));
    }
#line 2015 "yacc_sql.cpp"
    break;

  case 42: /* attr_def: ID type opt_null  */
#line 408 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      switch((yyval.attr_info)->type) {
        case AttrType::INTS:
        case AttrType::CHARS:
        case AttrType::FLOATS:
        case AttrType::DATES: {
          (yyval.attr_info)->length = 4;
        } break;
        case AttrType::TEXTS: {
          (yyval.attr_info)->length = 16;
        } break;
        default: {
          (yyval.attr_info)->length = 4;
        } break;
      }
      (yyval.attr_info)->nullable = (yyvsp[0].bools);
      free((yyvsp[-2].string));
    }
#line 2041 "yacc_sql.cpp"
    break;

  case 43: /* opt_null: %empty  */
#line 431 "yacc_sql.y"
    {
      (yyval.bools) = false;
    }
#line 2049 "yacc_sql.cpp"
    break;

  case 44: /* opt_null: NULLABLE_SYM  */
#line 434 "yacc_sql.y"
                   {
      (yyval.bools) = true;
    }
#line 2057 "yacc_sql.cpp"
    break;

  case 45: /* opt_null: NULL_SYM  */
#line 437 "yacc_sql.y"
               {
      (yyval.bools) = true;
    }
#line 2065 "yacc_sql.cpp"
    break;

  case 46: /* opt_null: NOT NULL_SYM  */
#line 440 "yacc_sql.y"
                   {
      (yyval.bools) = false;
    }
#line 2073 "yacc_sql.cpp"
    break;

  case 47: /* number: NUMBER  */
#line 445 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2079 "yacc_sql.cpp"
    break;

  case 48: /* type: INT_T  */
#line 448 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2085 "yacc_sql.cpp"
    break;

  case 49: /* type: STRING_T  */
#line 449 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2091 "yacc_sql.cpp"
    break;

  case 50: /* type: FLOAT_T  */
#line 450 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2097 "yacc_sql.cpp"
    break;

  case 51: /* type: DATE_T  */
#line 451 "yacc_sql.y"
              { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2103 "yacc_sql.cpp"
    break;

  case 52: /* type: TEXT_T  */
#line 452 "yacc_sql.y"
             { (yyval.number) = static_cast<int>(AttrType::TEXTS); }
#line 2109 "yacc_sql.cpp"
    break;

  case 53: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 456 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-5].string);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.sql_node)->insertion.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-2].value);
      free((yyvsp[-5].string));
    }
#line 2126 "yacc_sql.cpp"
    break;

  case 54: /* value_list: %empty  */
#line 472 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2134 "yacc_sql.cpp"
    break;

  case 55: /* value_list: COMMA value value_list  */
#line 475 "yacc_sql.y"
                              { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2148 "yacc_sql.cpp"
    break;

  case 56: /* value: '-' value  */
#line 486 "yacc_sql.y"
             {
      if((yyvsp[0].value)->attr_type() == AttrType::INTS){
        (yyval.value) = new Value(-1 * int((yyvsp[0].value)->get_int()));
      }
      else if((yyvsp[0].value)->attr_type() == AttrType::FLOATS){
        (yyval.value) = new Value(-1.0f * float((yyvsp[0].value)->get_float()));
      }
      else{
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("Cannot use negative operator to not number or not float one"));
        YYERROR;
      }
      delete (yyvsp[0].value);
    }
#line 2166 "yacc_sql.cpp"
    break;

  case 57: /* value: NULL_SYM  */
#line 499 "yacc_sql.y"
               {
      (yyval.value) = new Value;
      *((yyval.value)) = Value::Null(); /* NULL value */
      (yyloc) = (yylsp[0]);
    }
#line 2176 "yacc_sql.cpp"
    break;

  case 58: /* value: NUMBER  */
#line 504 "yacc_sql.y"
             {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2185 "yacc_sql.cpp"
    break;

  case 59: /* value: FLOAT  */
#line 508 "yacc_sql.y"
            {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2194 "yacc_sql.cpp"
    break;

  case 60: /* value: SSS  */
#line 512 "yacc_sql.y"
          {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 61: /* storage_format: %empty  */
#line 521 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2213 "yacc_sql.cpp"
    break;

  case 62: /* storage_format: STORAGE FORMAT EQ ID  */
#line 525 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2221 "yacc_sql.cpp"
    break;

  case 63: /* delete_stmt: DELETE FROM ID where  */
#line 532 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2235 "yacc_sql.cpp"
    break;

  case 64: /* update_stmt: UPDATE ID SET ID EQ value where  */
#line 544 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-5].string);
      (yyval.sql_node)->update.attribute_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.value = *(yyvsp[-1].value);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
      delete (yyvsp[-1].value);
    }
#line 2253 "yacc_sql.cpp"
    break;

  case 65: /* select_stmt: SELECT expression_list FROM table_ref_list where group_by opt_order_by  */
#line 560 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-5].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-5].expression_list));
        delete (yyvsp[-5].expression_list);
      }

      if ((yyvsp[-3].table_ref_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap((yyvsp[-3].table_ref_list)->relations);
        (yyval.sql_node)->selection.conditions.swap((yyvsp[-3].table_ref_list)->conditions);
        delete (yyvsp[-3].table_ref_list);
      }

      if ((yyvsp[-2].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.insert((yyval.sql_node)->selection.conditions.end(), (yyvsp[-2].condition_list)->begin(), (yyvsp[-2].condition_list)->end());
        delete (yyvsp[-2].condition_list);
      }

      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-1].expression_list));
        delete (yyvsp[-1].expression_list);
      }

      if ((yyvsp[0].order_by_list) != nullptr) {
        (yyval.sql_node)->selection.order_sql_nodes.swap(*(yyvsp[0].order_by_list));
        delete (yyvsp[0].order_by_list);
      }
    }
#line 2286 "yacc_sql.cpp"
    break;

  case 66: /* opt_order_by: %empty  */
#line 591 "yacc_sql.y"
  {
    (yyval.order_by_list) = nullptr;   // empty
  }
#line 2294 "yacc_sql.cpp"
    break;

  case 67: /* opt_order_by: ORDER BY order_by_list  */
#line 595 "yacc_sql.y"
  {
    (yyval.order_by_list) = (yyvsp[0].order_by_list);
  }
#line 2302 "yacc_sql.cpp"
    break;

  case 68: /* order_by_list: order_by  */
#line 602 "yacc_sql.y"
  {
    (yyval.order_by_list) = new std::vector<OrderSqlNode>;
    (yyval.order_by_list)->emplace_back(*(yyvsp[0].order_by));
    delete (yyvsp[0].order_by);
  }
#line 2312 "yacc_sql.cpp"
    break;

  case 69: /* order_by_list: order_by COMMA order_by_list  */
#line 608 "yacc_sql.y"
  {
    if ((yyvsp[0].order_by_list) != nullptr) {
        (yyval.order_by_list) = (yyvsp[0].order_by_list);
    }else {
        (yyval.order_by_list) = new std::vector<OrderSqlNode>;
    }
    (yyval.order_by_list)->emplace((yyval.order_by_list)->begin(), *(yyvsp[-2].order_by));
    delete (yyvsp[-2].order_by);
  }
#line 2326 "yacc_sql.cpp"
    break;

  case 70: /* order_by: expression  */
#line 621 "yacc_sql.y"
  {
    if((yyvsp[0].expression) == nullptr || (yyvsp[0].expression)->type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[0].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    (yyval.order_by) = new OrderSqlNode();
    (yyval.order_by)->inc_order = true;
    (yyval.order_by)->unbound_field_expr_ = (yyvsp[0].expression);
    (yyvsp[0].expression) = nullptr;
  }
#line 2342 "yacc_sql.cpp"
    break;

  case 71: /* order_by: expression ASC  */
#line 633 "yacc_sql.y"
  {
    if((yyvsp[-1].expression) == nullptr || (yyvsp[-1].expression)->type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    (yyval.order_by) = new OrderSqlNode();
    (yyval.order_by)->inc_order = true;
    (yyval.order_by)->unbound_field_expr_ = (yyvsp[-1].expression);
    (yyvsp[-1].expression) = nullptr;
  }
#line 2358 "yacc_sql.cpp"
    break;

  case 72: /* order_by: expression DESC  */
#line 645 "yacc_sql.y"
  {
   if((yyvsp[-1].expression) == nullptr || (yyvsp[-1].expression)->type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: order by field is invalid"));
        YYERROR;
    }
    (yyval.order_by) = new OrderSqlNode();
    (yyval.order_by)->inc_order = false;
    (yyval.order_by)->unbound_field_expr_ = (yyvsp[-1].expression);
    (yyvsp[-1].expression) = nullptr;
  }
#line 2374 "yacc_sql.cpp"
    break;

  case 73: /* calc_stmt: CALC expression_list  */
#line 660 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2384 "yacc_sql.cpp"
    break;

  case 74: /* expression_list: expression  */
#line 669 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2393 "yacc_sql.cpp"
    break;

  case 75: /* expression_list: expression COMMA expression_list  */
#line 674 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-2].expression));
    }
#line 2406 "yacc_sql.cpp"
    break;

  case 76: /* expression: '-' expression  */
#line 684 "yacc_sql.y"
                                {
      ValueExpr* vepr = new ValueExpr(Value((int)0));
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, vepr, (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2415 "yacc_sql.cpp"
    break;

  case 77: /* expression: expression '+' expression  */
#line 688 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2423 "yacc_sql.cpp"
    break;

  case 78: /* expression: expression '-' expression  */
#line 691 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2431 "yacc_sql.cpp"
    break;

  case 79: /* expression: expression '*' expression  */
#line 694 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2439 "yacc_sql.cpp"
    break;

  case 80: /* expression: expression '/' expression  */
#line 697 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2447 "yacc_sql.cpp"
    break;

  case 81: /* expression: LBRACE expression RBRACE  */
#line 700 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2456 "yacc_sql.cpp"
    break;

  case 82: /* expression: value  */
#line 704 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2466 "yacc_sql.cpp"
    break;

  case 83: /* expression: rel_attr  */
#line 709 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2477 "yacc_sql.cpp"
    break;

  case 84: /* expression: '*'  */
#line 715 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2485 "yacc_sql.cpp"
    break;

  case 85: /* expression: MAX LBRACE expression RBRACE  */
#line 719 "yacc_sql.y"
                                  {
      if((yyvsp[-1].expression) -> type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support MAX(FIELD)"));
        YYERROR;
      }else{
        (yyval.expression) = create_aggregate_expression("MAX", (yyvsp[-1].expression), sql_string, &(yyloc));
      }
    }
#line 2499 "yacc_sql.cpp"
    break;

  case 86: /* expression: MIN LBRACE expression RBRACE  */
#line 728 "yacc_sql.y"
                                  {
      if((yyvsp[-1].expression) -> type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support MIN(FIELD)"));
        YYERROR;
      }else{
        (yyval.expression) = create_aggregate_expression("MIN", (yyvsp[-1].expression), sql_string, &(yyloc));
      }
    }
#line 2513 "yacc_sql.cpp"
    break;

  case 87: /* expression: SUM LBRACE expression RBRACE  */
#line 737 "yacc_sql.y"
                                  {
      if((yyvsp[-1].expression) -> type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support SUM(FIELD)"));
        YYERROR;
      }else{
        (yyval.expression) = create_aggregate_expression("SUM", (yyvsp[-1].expression), sql_string, &(yyloc));
      }
    }
#line 2527 "yacc_sql.cpp"
    break;

  case 88: /* expression: AVG LBRACE expression RBRACE  */
#line 746 "yacc_sql.y"
                                  {
      if((yyvsp[-1].expression) -> type() != ExprType::UNBOUND_FIELD){
        delete (yyvsp[-1].expression);
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: can only support AVG(FIELD)"));
        YYERROR;
      }else{
        (yyval.expression) = create_aggregate_expression("AVG", (yyvsp[-1].expression), sql_string, &(yyloc));
      }
    }
#line 2541 "yacc_sql.cpp"
    break;

  case 89: /* expression: COUNT LBRACE expression RBRACE  */
#line 755 "yacc_sql.y"
                                    {
      (yyval.expression) = create_aggregate_expression("COUNT", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2549 "yacc_sql.cpp"
    break;

  case 90: /* rel_attr: ID  */
#line 761 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2559 "yacc_sql.cpp"
    break;

  case 91: /* rel_attr: ID DOT ID  */
#line 766 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2571 "yacc_sql.cpp"
    break;

  case 92: /* relation: ID  */
#line 776 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2579 "yacc_sql.cpp"
    break;

  case 93: /* table_ref_list: comma_ref_list  */
#line 781 "yacc_sql.y"
                   {  // 返回逗号连接的表列表
      (yyval.table_ref_list) = (yyvsp[0].table_ref_list);
    }
#line 2587 "yacc_sql.cpp"
    break;

  case 94: /* table_ref_list: join_ref_list  */
#line 784 "yacc_sql.y"
                    { // 返回 INNER JOIN 的表列表
      (yyval.table_ref_list) = (yyvsp[0].table_ref_list);
    }
#line 2595 "yacc_sql.cpp"
    break;

  case 95: /* comma_ref_list: relation  */
#line 789 "yacc_sql.y"
             {
      (yyval.table_ref_list) = new TableRefSqlNode();
      (yyval.table_ref_list)->relations.push_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 2605 "yacc_sql.cpp"
    break;

  case 96: /* comma_ref_list: relation COMMA table_ref_list  */
#line 794 "yacc_sql.y"
                                    {
      if ((yyvsp[0].table_ref_list) != nullptr) {
        (yyval.table_ref_list) = (yyvsp[0].table_ref_list);
      } else {
        (yyval.table_ref_list) = new TableRefSqlNode();
      }

      (yyval.table_ref_list)->relations.insert((yyval.table_ref_list)->relations.begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 2620 "yacc_sql.cpp"
    break;

  case 97: /* join_ref_list: relation INNER JOIN relation ON condition_list  */
#line 806 "yacc_sql.y"
                                                   {
      (yyval.table_ref_list) = new TableRefSqlNode();

      (yyval.table_ref_list)->relations.push_back((yyvsp[-5].string));  // 左侧的relation
      (yyval.table_ref_list)->relations.push_back((yyvsp[-2].string));  // 右侧的relation
      free((yyvsp[-5].string));
      free((yyvsp[-2].string));

      // 将ON条件存储为 conditions
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.table_ref_list)->conditions.insert((yyval.table_ref_list)->conditions.end(), (yyvsp[0].condition_list)->begin(), (yyvsp[0].condition_list)->end());
        delete (yyvsp[0].condition_list);
      }
    }
#line 2639 "yacc_sql.cpp"
    break;

  case 98: /* join_ref_list: join_ref_list INNER JOIN relation ON condition_list  */
#line 820 "yacc_sql.y"
                                                          {
      // 处理嵌套的 INNER JOIN
      if ((yyvsp[-5].table_ref_list) != nullptr) {
        (yyval.table_ref_list) = (yyvsp[-5].table_ref_list);  // 左侧的rel_list
      } else {
        (yyval.table_ref_list) = new TableRefSqlNode();
      }

      (yyval.table_ref_list)->relations.push_back((yyvsp[-2].string));  // 右侧的relation
      free((yyvsp[-2].string));

      // 将ON条件存储为 conditions
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.table_ref_list)->conditions.insert((yyval.table_ref_list)->conditions.end(), (yyvsp[0].condition_list)->begin(), (yyvsp[0].condition_list)->end());
        delete (yyvsp[0].condition_list);
      }
    }
#line 2661 "yacc_sql.cpp"
    break;

  case 99: /* where: %empty  */
#line 841 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2669 "yacc_sql.cpp"
    break;

  case 100: /* where: WHERE condition_list  */
#line 844 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2677 "yacc_sql.cpp"
    break;

  case 101: /* condition_list: %empty  */
#line 850 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2685 "yacc_sql.cpp"
    break;

  case 102: /* condition_list: condition  */
#line 853 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2695 "yacc_sql.cpp"
    break;

  case 103: /* condition_list: condition AND condition_list  */
#line 858 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2705 "yacc_sql.cpp"
    break;

  case 104: /* condition: expression comp_op expression  */
#line 866 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
    }
#line 2716 "yacc_sql.cpp"
    break;

  case 105: /* condition: expression is_null_comp  */
#line 873 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-1].expression);
      (yyval.condition)->comp = (yyvsp[0].bools) ? IS_NULL_OP : NOT_NULL_OP;
      (yyval.condition)->right_expr = new ValueExpr(Value::Null());
    }
#line 2727 "yacc_sql.cpp"
    break;

  case 106: /* is_null_comp: IS_SYM NULL_SYM  */
#line 881 "yacc_sql.y"
                    {
      return true;
    }
#line 2735 "yacc_sql.cpp"
    break;

  case 107: /* is_null_comp: IS_SYM NOT NULL_SYM  */
#line 884 "yacc_sql.y"
                          {
      return false;
    }
#line 2743 "yacc_sql.cpp"
    break;

  case 108: /* comp_op: EQ  */
#line 890 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2749 "yacc_sql.cpp"
    break;

  case 109: /* comp_op: LT  */
#line 891 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2755 "yacc_sql.cpp"
    break;

  case 110: /* comp_op: GT  */
#line 892 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2761 "yacc_sql.cpp"
    break;

  case 111: /* comp_op: LE  */
#line 893 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2767 "yacc_sql.cpp"
    break;

  case 112: /* comp_op: GE  */
#line 894 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2773 "yacc_sql.cpp"
    break;

  case 113: /* comp_op: NE  */
#line 895 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2779 "yacc_sql.cpp"
    break;

  case 114: /* comp_op: LIKE  */
#line 896 "yacc_sql.y"
           { (yyval.comp) = LIKE_OP; }
#line 2785 "yacc_sql.cpp"
    break;

  case 115: /* comp_op: NOT LIKE  */
#line 897 "yacc_sql.y"
               { (yyval.comp) = NO_LIKE_OP; }
#line 2791 "yacc_sql.cpp"
    break;

  case 116: /* group_by: %empty  */
#line 903 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2799 "yacc_sql.cpp"
    break;

  case 117: /* group_by: GROUP BY expression_list  */
#line 907 "yacc_sql.y"
    {
        (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2807 "yacc_sql.cpp"
    break;

  case 118: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 913 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 2821 "yacc_sql.cpp"
    break;

  case 119: /* explain_stmt: EXPLAIN command_wrapper  */
#line 926 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 2830 "yacc_sql.cpp"
    break;

  case 120: /* set_variable_stmt: SET ID EQ value  */
#line 934 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2842 "yacc_sql.cpp"
    break;


#line 2846 "yacc_sql.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 946 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
