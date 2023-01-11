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
#line 1 "parsor_bison.y"

#include <iostream>
#include <memory>
#include "ast.h"
#include "parsor_bison.tab.h"

void yyerror(YYLTYPE *locp, const char* s){
    std::cerr << "Parser Error at LINE " << locp->first_line << " COLUMN " << locp->first_column << ":" << s << std::endl;
}

extern int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

using namespace ast;


#line 87 "parsor_bison.tab.c"

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

#include "parsor_bison.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_EXIT = 3,                       /* EXIT  */
  YYSYMBOL_HELP = 4,                       /* HELP  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_DROP = 7,                       /* DROP  */
  YYSYMBOL_SHOW = 8,                       /* SHOW  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_CHAR = 11,                      /* CHAR  */
  YYSYMBOL_PRIMARY = 12,                   /* PRIMARY  */
  YYSYMBOL_KEY = 13,                       /* KEY  */
  YYSYMBOL_sNULL = 14,                     /* sNULL  */
  YYSYMBOL_SELECT = 15,                    /* SELECT  */
  YYSYMBOL_FROM = 16,                      /* FROM  */
  YYSYMBOL_WHERE = 17,                     /* WHERE  */
  YYSYMBOL_INSERT = 18,                    /* INSERT  */
  YYSYMBOL_INTO = 19,                      /* INTO  */
  YYSYMBOL_VALUES = 20,                    /* VALUES  */
  YYSYMBOL_DELETE = 21,                    /* DELETE  */
  YYSYMBOL_UPDATE = 22,                    /* UPDATE  */
  YYSYMBOL_SET = 23,                       /* SET  */
  YYSYMBOL_AS = 24,                        /* AS  */
  YYSYMBOL_AND = 25,                       /* AND  */
  YYSYMBOL_OR = 26,                        /* OR  */
  YYSYMBOL_NOT = 27,                       /* NOT  */
  YYSYMBOL_NEQ = 28,                       /* NEQ  */
  YYSYMBOL_LE = 29,                        /* LE  */
  YYSYMBOL_GE = 30,                        /* GE  */
  YYSYMBOL_CONST_INT = 31,                 /* CONST_INT  */
  YYSYMBOL_CONST_STRING = 32,              /* CONST_STRING  */
  YYSYMBOL_CONST_FLOAT = 33,               /* CONST_FLOAT  */
  YYSYMBOL_IDENTIFIER = 34,                /* IDENTIFIER  */
  YYSYMBOL_35_ = 35,                       /* '<'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '='  */
  YYSYMBOL_38_ = 38,                       /* '+'  */
  YYSYMBOL_39_ = 39,                       /* '-'  */
  YYSYMBOL_40_ = 40,                       /* '*'  */
  YYSYMBOL_41_ = 41,                       /* '/'  */
  YYSYMBOL_UMINUS = 42,                    /* UMINUS  */
  YYSYMBOL_43_ = 43,                       /* ';'  */
  YYSYMBOL_44_ = 44,                       /* ','  */
  YYSYMBOL_45_ = 45,                       /* '('  */
  YYSYMBOL_46_ = 46,                       /* ')'  */
  YYSYMBOL_47_ = 47,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_menu = 49,                      /* menu  */
  YYSYMBOL_stmt = 50,                      /* stmt  */
  YYSYMBOL_value = 51,                     /* value  */
  YYSYMBOL_valueList = 52,                 /* valueList  */
  YYSYMBOL_colNameList = 53,               /* colNameList  */
  YYSYMBOL_opt_colNameList = 54,           /* opt_colNameList  */
  YYSYMBOL_col = 55,                       /* col  */
  YYSYMBOL_opt_as = 56,                    /* opt_as  */
  YYSYMBOL_selcol = 57,                    /* selcol  */
  YYSYMBOL_selcolList = 58,                /* selcolList  */
  YYSYMBOL_fromTab = 59,                   /* fromTab  */
  YYSYMBOL_tabList = 60,                   /* tabList  */
  YYSYMBOL_dataType = 61,                  /* dataType  */
  YYSYMBOL_option = 62,                    /* option  */
  YYSYMBOL_optionList = 63,                /* optionList  */
  YYSYMBOL_opt_Option = 64,                /* opt_Option  */
  YYSYMBOL_field = 65,                     /* field  */
  YYSYMBOL_fieldList = 66,                 /* fieldList  */
  YYSYMBOL_upd = 67,                       /* upd  */
  YYSYMBOL_updList = 68,                   /* updList  */
  YYSYMBOL_dml = 69,                       /* dml  */
  YYSYMBOL_ddl = 70,                       /* ddl  */
  YYSYMBOL_selList = 71,                   /* selList  */
  YYSYMBOL_opt_where = 72,                 /* opt_where  */
  YYSYMBOL_expr = 73,                      /* expr  */
  YYSYMBOL_sys = 74,                       /* sys  */
  YYSYMBOL_tabName = 75,                   /* tabName  */
  YYSYMBOL_colName = 76,                   /* colName  */
  YYSYMBOL_altName = 77                    /* altName  */
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   163

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  140

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   290


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
      45,    46,    40,    38,    44,    39,    47,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    43,
      35,    37,    36,     2,     2,     2,     2,     2,     2,     2,
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
      42
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    75,    75,    82,    84,    86,    90,    94,    98,   104,
     108,   114,   118,   125,   128,   134,   138,   145,   148,   152,
     167,   173,   177,   183,   187,   191,   197,   201,   207,   211,
     215,   221,   225,   231,   235,   242,   245,   251,   257,   261,
     267,   273,   277,   283,   287,   291,   295,   301,   305,   309,
     315,   319,   323,   326,   332,   337,   341,   345,   349,   353,
     357,   361,   365,   369,   373,   377,   381,   385,   389,   393,
     397,   403,   407,   415,   416,   417
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "EXIT", "HELP",
  "CREATE", "TABLE", "DROP", "SHOW", "INT", "FLOAT", "CHAR", "PRIMARY",
  "KEY", "sNULL", "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES",
  "DELETE", "UPDATE", "SET", "AS", "AND", "OR", "NOT", "NEQ", "LE", "GE",
  "CONST_INT", "CONST_STRING", "CONST_FLOAT", "IDENTIFIER", "'<'", "'>'",
  "'='", "'+'", "'-'", "'*'", "'/'", "UMINUS", "';'", "','", "'('", "')'",
  "'.'", "$accept", "menu", "stmt", "value", "valueList", "colNameList",
  "opt_colNameList", "col", "opt_as", "selcol", "selcolList", "fromTab",
  "tabList", "dataType", "option", "optionList", "opt_Option", "field",
  "fieldList", "upd", "updList", "dml", "ddl", "selList", "opt_where",
  "expr", "sys", "tabName", "colName", "altName", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-89)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-74)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      82,   -89,   -89,    11,    21,    30,   -30,    22,    26,    15,
      52,     7,   -89,   -89,   -89,    15,    15,   -89,    37,   -89,
     -13,   -89,     9,    53,    48,   -89,    15,    15,   -89,    75,
     -89,   -89,    54,   -89,    68,   -89,   -89,   -89,    76,    15,
      83,    64,   101,    76,    83,   -89,   -89,   -89,    -5,    -9,
     -89,   -89,    83,   100,    74,   -89,    92,   -89,    -4,   -89,
     -16,    65,    15,   -89,    68,   -89,    50,   -89,    85,    74,
     -89,   -89,   -89,    74,    74,   -89,   -89,    86,    74,    76,
     -89,    83,   -89,   -89,   -89,   104,    -3,   -89,   -89,    83,
     -89,    60,   117,   -89,    42,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    86,   -89,   -89,
     106,   122,   134,   -89,    -3,   -89,   -89,   -89,    90,   -89,
     117,   103,     6,     6,     6,     6,     6,     6,   -18,   -18,
     -89,   -89,   105,   -89,   -89,   -89,    60,   -89,   -89,   -89
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    71,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     3,     0,     0,    49,    74,    50,
      17,    21,    51,     0,     0,    16,     0,     0,    73,     0,
       1,     2,     0,    48,     0,    75,    20,    19,     0,     0,
       0,    13,    52,     0,     0,    18,    22,    26,    52,    25,
      74,    15,     0,     0,     0,    45,     0,    41,    52,    38,
       0,     0,     0,    43,     0,    24,     0,    11,     0,     0,
       6,     8,     7,     0,     0,    54,    55,    53,     0,     0,
      46,     0,    47,    28,    29,     0,    35,    27,    23,     0,
      14,     0,    69,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    42,    39,
       0,     0,     0,    33,    36,    37,    12,     9,     0,    56,
      67,    68,    62,    64,    66,    63,    65,    61,    57,    58,
      59,    60,     0,    32,    31,    34,     0,    44,    30,    10
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -89,   -89,   -89,   -88,   -89,   -89,   -89,    -6,   -89,   112,
     -89,    97,   -89,   -89,    46,   -89,   -89,    80,   -89,    84,
     -89,   -89,   -89,   -89,   -32,   -40,   -89,    -8,   -38,   -29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    10,    11,    75,   118,    66,    53,    76,    36,    21,
      22,    47,    48,    86,   113,   114,   115,    59,    60,    57,
      58,    12,    13,    23,    55,    77,    14,    24,    25,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,    29,    51,   117,    18,    45,    61,    32,    33,   111,
      19,    34,    54,    54,    67,    64,    63,    15,    41,    42,
      65,    35,   105,   106,   112,    35,    80,    16,    81,    92,
      82,    49,    20,    93,    94,    88,    17,    56,   107,    62,
      79,    26,    27,    61,   103,   104,   105,   106,   139,    28,
      31,   116,    30,    38,    49,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,    95,    96,    39,
      97,    98,    99,    56,    83,    84,    85,   100,   101,   102,
     103,   104,   105,   106,   -73,     1,     2,     3,   119,     4,
       5,    70,    71,    72,    89,    40,    90,     6,    43,    44,
       7,    69,    35,     8,     9,    70,    71,    72,    18,    52,
      18,    95,    96,    73,    97,    98,    99,    50,    54,    74,
      68,   100,   101,   102,   103,   104,   105,   106,    95,    78,
      91,    97,    98,    99,   136,   133,   137,   132,   100,   101,
     102,   103,   104,   105,   106,    97,    98,    99,   134,   110,
      46,   138,   100,   101,   102,   103,   104,   105,   106,    87,
     135,   109,     0,   108
};

static const yytype_int16 yycheck[] =
{
       6,     9,    40,    91,    34,    34,    44,    15,    16,    12,
      40,    24,    17,    17,    52,    24,    48,     6,    26,    27,
      49,    34,    40,    41,    27,    34,    58,     6,    44,    69,
      46,    39,    38,    73,    74,    64,     6,    43,    78,    44,
      44,    19,    16,    81,    38,    39,    40,    41,   136,    34,
      43,    89,     0,    44,    62,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    25,    26,    16,
      28,    29,    30,    79,     9,    10,    11,    35,    36,    37,
      38,    39,    40,    41,    47,     3,     4,     5,    46,     7,
       8,    31,    32,    33,    44,    47,    46,    15,    23,    45,
      18,    27,    34,    21,    22,    31,    32,    33,    34,    45,
      34,    25,    26,    39,    28,    29,    30,    34,    17,    45,
      20,    35,    36,    37,    38,    39,    40,    41,    25,    37,
      45,    28,    29,    30,    44,    13,    46,    31,    35,    36,
      37,    38,    39,    40,    41,    28,    29,    30,    14,    45,
      38,    46,    35,    36,    37,    38,    39,    40,    41,    62,
     114,    81,    -1,    79
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,     8,    15,    18,    21,    22,
      49,    50,    69,    70,    74,     6,     6,     6,    34,    40,
      55,    57,    58,    71,    75,    76,    19,    16,    34,    75,
       0,    43,    75,    75,    24,    34,    56,    77,    44,    16,
      47,    75,    75,    23,    45,    77,    57,    59,    60,    75,
      34,    76,    45,    54,    17,    72,    55,    67,    68,    65,
      66,    76,    44,    72,    24,    77,    53,    76,    20,    27,
      31,    32,    33,    39,    45,    51,    55,    73,    37,    44,
      72,    44,    46,     9,    10,    11,    61,    59,    77,    44,
      46,    45,    73,    73,    73,    25,    26,    28,    29,    30,
      35,    36,    37,    38,    39,    40,    41,    73,    67,    65,
      45,    12,    27,    62,    63,    64,    76,    51,    52,    46,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    31,    13,    14,    62,    44,    46,    46,    51
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    50,    51,    51,    51,    52,
      52,    53,    53,    54,    54,    55,    55,    56,    56,    56,
      57,    58,    58,    59,    59,    59,    60,    60,    61,    61,
      61,    62,    62,    63,    63,    64,    64,    65,    66,    66,
      67,    68,    68,    69,    69,    69,    69,    70,    70,    70,
      71,    71,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    74,    75,    76,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     0,     3,     3,     1,     0,     2,     1,
       2,     1,     3,     3,     2,     1,     1,     3,     1,     1,
       4,     2,     2,     1,     2,     0,     1,     3,     1,     3,
       3,     1,     3,     5,     8,     4,     5,     6,     3,     2,
       1,     1,     0,     2,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     1,     1,     1,     1,     1
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
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
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
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
                 int yyrule)
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
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
yyparse (void)
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
      yychar = yylex (&yylval, &yylloc);
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
  case 2: /* menu: stmt ';'  */
#line 76 "parsor_bison.y"
        {
            ast::parse_tree = (yyvsp[-1].sv_node);
            YYACCEPT;
        }
#line 1373 "parsor_bison.tab.c"
    break;

  case 3: /* stmt: sys  */
#line 83 "parsor_bison.y"
        {}
#line 1379 "parsor_bison.tab.c"
    break;

  case 4: /* stmt: dml  */
#line 85 "parsor_bison.y"
        {}
#line 1385 "parsor_bison.tab.c"
    break;

  case 5: /* stmt: ddl  */
#line 87 "parsor_bison.y"
        {}
#line 1391 "parsor_bison.tab.c"
    break;

  case 6: /* value: CONST_INT  */
#line 91 "parsor_bison.y"
        {
            (yyval.sv_value) = std::make_shared<ConstInt>((yyvsp[0].sv_int));
        }
#line 1399 "parsor_bison.tab.c"
    break;

  case 7: /* value: CONST_FLOAT  */
#line 95 "parsor_bison.y"
        {
            (yyval.sv_value) = std::make_shared<ConstFloat>((yyvsp[0].sv_float));
        }
#line 1407 "parsor_bison.tab.c"
    break;

  case 8: /* value: CONST_STRING  */
#line 99 "parsor_bison.y"
        {
            (yyval.sv_value) = std::make_shared<ConstString>((yyvsp[0].sv_string));
        }
#line 1415 "parsor_bison.tab.c"
    break;

  case 9: /* valueList: value  */
#line 105 "parsor_bison.y"
        {
            (yyval.sv_values) = std::vector<std::shared_ptr<ast::ConstValue>>{(yyvsp[0].sv_value)};
        }
#line 1423 "parsor_bison.tab.c"
    break;

  case 10: /* valueList: valueList ',' value  */
#line 109 "parsor_bison.y"
        {
            (yyval.sv_values).push_back((yyvsp[0].sv_value));
        }
#line 1431 "parsor_bison.tab.c"
    break;

  case 11: /* colNameList: colName  */
#line 115 "parsor_bison.y"
        {
            (yyval.sv_colnames) = std::vector<std::string>{(yyvsp[0].sv_string)};
        }
#line 1439 "parsor_bison.tab.c"
    break;

  case 12: /* colNameList: colNameList ',' colName  */
#line 119 "parsor_bison.y"
        {
            (yyval.sv_colnames).push_back((yyvsp[0].sv_string));
        }
#line 1447 "parsor_bison.tab.c"
    break;

  case 13: /* opt_colNameList: %empty  */
#line 125 "parsor_bison.y"
        {
            (yyval.sv_colnames) = {};
        }
#line 1455 "parsor_bison.tab.c"
    break;

  case 14: /* opt_colNameList: '(' colNameList ')'  */
#line 129 "parsor_bison.y"
        {
            (yyval.sv_colnames) = (yyvsp[-1].sv_colnames);
        }
#line 1463 "parsor_bison.tab.c"
    break;

  case 15: /* col: tabName '.' colName  */
#line 135 "parsor_bison.y"
        {
            (yyval.sv_column) = std::make_shared<ast::Column>((yyvsp[-2].sv_string), (yyvsp[0].sv_string));
        }
#line 1471 "parsor_bison.tab.c"
    break;

  case 16: /* col: colName  */
#line 139 "parsor_bison.y"
        {
            (yyval.sv_column) = std::make_shared<ast::Column>("", (yyvsp[0].sv_string));
        }
#line 1479 "parsor_bison.tab.c"
    break;

  case 17: /* opt_as: %empty  */
#line 145 "parsor_bison.y"
        {
            (yyval.sv_string) = "";
        }
#line 1487 "parsor_bison.tab.c"
    break;

  case 18: /* opt_as: AS altName  */
#line 149 "parsor_bison.y"
        {
            (yyval.sv_string) = (yyvsp[0].sv_string);
        }
#line 1495 "parsor_bison.tab.c"
    break;

  case 19: /* opt_as: altName  */
#line 153 "parsor_bison.y"
        {
            (yyval.sv_string) = (yyvsp[0].sv_string); 
        }
#line 1503 "parsor_bison.tab.c"
    break;

  case 20: /* selcol: col opt_as  */
#line 168 "parsor_bison.y"
        {
            (yyval.sv_selcol) = std::make_shared<ast::SelColumn>((yyvsp[-1].sv_column),(yyvsp[0].sv_string));
        }
#line 1511 "parsor_bison.tab.c"
    break;

  case 21: /* selcolList: selcol  */
#line 174 "parsor_bison.y"
        {
            (yyval.sv_selcols) = std::vector<std::shared_ptr<ast::SelColumn>>{(yyvsp[0].sv_selcol)};
        }
#line 1519 "parsor_bison.tab.c"
    break;

  case 22: /* selcolList: selcolList ',' selcol  */
#line 178 "parsor_bison.y"
        {
            (yyval.sv_selcols).push_back((yyvsp[0].sv_selcol));
        }
#line 1527 "parsor_bison.tab.c"
    break;

  case 23: /* fromTab: tabName AS altName  */
#line 184 "parsor_bison.y"
        {
            (yyval.sv_fromtab) = std::make_shared<ast::FromTable>((yyvsp[-2].sv_string),(yyvsp[0].sv_string));
        }
#line 1535 "parsor_bison.tab.c"
    break;

  case 24: /* fromTab: tabName altName  */
#line 188 "parsor_bison.y"
        {
            (yyval.sv_fromtab) = std::make_shared<ast::FromTable>((yyvsp[-1].sv_string),(yyvsp[0].sv_string));
        }
#line 1543 "parsor_bison.tab.c"
    break;

  case 25: /* fromTab: tabName  */
#line 192 "parsor_bison.y"
        {
            (yyval.sv_fromtab) = std::make_shared<ast::FromTable>((yyvsp[0].sv_string),"");
        }
#line 1551 "parsor_bison.tab.c"
    break;

  case 26: /* tabList: fromTab  */
#line 198 "parsor_bison.y"
        {
            (yyval.sv_fromtabs) = std::vector<std::shared_ptr<FromTable>>{(yyvsp[0].sv_fromtab)};
        }
#line 1559 "parsor_bison.tab.c"
    break;

  case 27: /* tabList: tabList ',' fromTab  */
#line 202 "parsor_bison.y"
        {
            (yyval.sv_fromtabs).push_back((yyvsp[0].sv_fromtab));
        }
#line 1567 "parsor_bison.tab.c"
    break;

  case 28: /* dataType: INT  */
#line 208 "parsor_bison.y"
        {
            (yyval.sv_datatype) = {FIELD_INT,4};
        }
#line 1575 "parsor_bison.tab.c"
    break;

  case 29: /* dataType: FLOAT  */
#line 212 "parsor_bison.y"
        {
            (yyval.sv_datatype) = {FIELD_FLOAT,4};
        }
#line 1583 "parsor_bison.tab.c"
    break;

  case 30: /* dataType: CHAR '(' CONST_INT ')'  */
#line 216 "parsor_bison.y"
        {
            (yyval.sv_datatype) = {FIELD_CHAR,(yyvsp[-1].sv_int)};
        }
#line 1591 "parsor_bison.tab.c"
    break;

  case 31: /* option: NOT sNULL  */
#line 222 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = FIELD_OPT_NOTNULL;
        }
#line 1599 "parsor_bison.tab.c"
    break;

  case 32: /* option: PRIMARY KEY  */
#line 226 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = FIELD_OPT_PRIMKEY;
        }
#line 1607 "parsor_bison.tab.c"
    break;

  case 33: /* optionList: option  */
#line 232 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = (yyvsp[0].sv_fieldoption);
        }
#line 1615 "parsor_bison.tab.c"
    break;

  case 34: /* optionList: optionList option  */
#line 236 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = ((yyvsp[-1].sv_fieldoption) | (yyvsp[0].sv_fieldoption));
        }
#line 1623 "parsor_bison.tab.c"
    break;

  case 35: /* opt_Option: %empty  */
#line 242 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = 0;
        }
#line 1631 "parsor_bison.tab.c"
    break;

  case 36: /* opt_Option: optionList  */
#line 246 "parsor_bison.y"
        {
            (yyval.sv_fieldoption) = (yyvsp[0].sv_fieldoption);
        }
#line 1639 "parsor_bison.tab.c"
    break;

  case 37: /* field: colName dataType opt_Option  */
#line 252 "parsor_bison.y"
        {
            (yyval.sv_fieldinfo) = std::make_shared<ast::FieldInfo>((yyvsp[-2].sv_string),(yyvsp[-1].sv_datatype),(yyvsp[0].sv_fieldoption));
        }
#line 1647 "parsor_bison.tab.c"
    break;

  case 38: /* fieldList: field  */
#line 258 "parsor_bison.y"
        {
            (yyval.sv_fieldinfos) = std::vector<std::shared_ptr<ast::FieldInfo>>{(yyvsp[0].sv_fieldinfo)};
        }
#line 1655 "parsor_bison.tab.c"
    break;

  case 39: /* fieldList: fieldList ',' field  */
#line 262 "parsor_bison.y"
        {
            (yyval.sv_fieldinfos).push_back((yyvsp[0].sv_fieldinfo));
        }
#line 1663 "parsor_bison.tab.c"
    break;

  case 40: /* upd: col '=' expr  */
#line 268 "parsor_bison.y"
        {
            (yyval.sv_updexpr) = std::make_shared<ast::UpdateExpr>((yyvsp[-2].sv_column),(yyvsp[0].sv_expression));
        }
#line 1671 "parsor_bison.tab.c"
    break;

  case 41: /* updList: upd  */
#line 274 "parsor_bison.y"
        {
            (yyval.sv_updexprs) = std::vector<std::shared_ptr<ast::UpdateExpr>>{(yyvsp[0].sv_updexpr)};
        }
#line 1679 "parsor_bison.tab.c"
    break;

  case 42: /* updList: updList ',' upd  */
#line 278 "parsor_bison.y"
        {
            (yyval.sv_updexprs).push_back((yyvsp[0].sv_updexpr));
        }
#line 1687 "parsor_bison.tab.c"
    break;

  case 43: /* dml: SELECT selList FROM tabList opt_where  */
#line 284 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::SelectStmt>((yyvsp[-3].sv_selcols), (yyvsp[-1].sv_fromtabs), (yyvsp[0].sv_expression));
        }
#line 1695 "parsor_bison.tab.c"
    break;

  case 44: /* dml: INSERT INTO tabName opt_colNameList VALUES '(' valueList ')'  */
#line 288 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::InsertStmt>((yyvsp[-5].sv_string),(yyvsp[-4].sv_colnames),(yyvsp[-1].sv_values));
        }
#line 1703 "parsor_bison.tab.c"
    break;

  case 45: /* dml: DELETE FROM tabName opt_where  */
#line 292 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::DeleteStmt>((yyvsp[-1].sv_string),(yyvsp[0].sv_expression));
        }
#line 1711 "parsor_bison.tab.c"
    break;

  case 46: /* dml: UPDATE tabName SET updList opt_where  */
#line 296 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::UpdateStmt>((yyvsp[-3].sv_string),(yyvsp[-1].sv_updexprs),(yyvsp[0].sv_expression));
        }
#line 1719 "parsor_bison.tab.c"
    break;

  case 47: /* ddl: CREATE TABLE tabName '(' fieldList ')'  */
#line 302 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::CreateTableStmt>((yyvsp[-3].sv_string), (yyvsp[-1].sv_fieldinfos));
        }
#line 1727 "parsor_bison.tab.c"
    break;

  case 48: /* ddl: DROP TABLE tabName  */
#line 306 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::DropTableStmt>((yyvsp[0].sv_string));
        }
#line 1735 "parsor_bison.tab.c"
    break;

  case 49: /* ddl: SHOW TABLE  */
#line 310 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::ShowTableStmt>();
        }
#line 1743 "parsor_bison.tab.c"
    break;

  case 50: /* selList: '*'  */
#line 316 "parsor_bison.y"
        {
            (yyval.sv_selcols) = {};
        }
#line 1751 "parsor_bison.tab.c"
    break;

  case 52: /* opt_where: %empty  */
#line 323 "parsor_bison.y"
        { 
            (yyval.sv_expression) = std::make_shared<ConstInt>(1);
        }
#line 1759 "parsor_bison.tab.c"
    break;

  case 53: /* opt_where: WHERE expr  */
#line 327 "parsor_bison.y"
        {
            (yyval.sv_expression) = (yyvsp[0].sv_expression);
        }
#line 1767 "parsor_bison.tab.c"
    break;

  case 54: /* expr: value  */
#line 333 "parsor_bison.y"
        {
            (yyval.sv_expression) = (yyvsp[0].sv_value);
        }
#line 1775 "parsor_bison.tab.c"
    break;

  case 55: /* expr: col  */
#line 338 "parsor_bison.y"
        {
            (yyval.sv_expression) = (yyvsp[0].sv_column);
        }
#line 1783 "parsor_bison.tab.c"
    break;

  case 56: /* expr: '(' expr ')'  */
#line 342 "parsor_bison.y"
        {
            (yyval.sv_expression) = (yyvsp[-1].sv_expression);
        }
#line 1791 "parsor_bison.tab.c"
    break;

  case 57: /* expr: expr '+' expr  */
#line 346 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_ADD,(yyvsp[0].sv_expression));
        }
#line 1799 "parsor_bison.tab.c"
    break;

  case 58: /* expr: expr '-' expr  */
#line 350 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_SUB,(yyvsp[0].sv_expression));
        }
#line 1807 "parsor_bison.tab.c"
    break;

  case 59: /* expr: expr '*' expr  */
#line 354 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_MUL,(yyvsp[0].sv_expression));
        }
#line 1815 "parsor_bison.tab.c"
    break;

  case 60: /* expr: expr '/' expr  */
#line 358 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_DIV,(yyvsp[0].sv_expression));
        }
#line 1823 "parsor_bison.tab.c"
    break;

  case 61: /* expr: expr '=' expr  */
#line 362 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_EQU,(yyvsp[0].sv_expression));
        }
#line 1831 "parsor_bison.tab.c"
    break;

  case 62: /* expr: expr NEQ expr  */
#line 366 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_NEQ,(yyvsp[0].sv_expression));
        }
#line 1839 "parsor_bison.tab.c"
    break;

  case 63: /* expr: expr '<' expr  */
#line 370 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_L,(yyvsp[0].sv_expression));
        }
#line 1847 "parsor_bison.tab.c"
    break;

  case 64: /* expr: expr LE expr  */
#line 374 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_LE,(yyvsp[0].sv_expression));
        }
#line 1855 "parsor_bison.tab.c"
    break;

  case 65: /* expr: expr '>' expr  */
#line 378 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_G,(yyvsp[0].sv_expression));
        }
#line 1863 "parsor_bison.tab.c"
    break;

  case 66: /* expr: expr GE expr  */
#line 382 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_GE,(yyvsp[0].sv_expression));
        }
#line 1871 "parsor_bison.tab.c"
    break;

  case 67: /* expr: expr AND expr  */
#line 386 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_AND,(yyvsp[0].sv_expression));
        }
#line 1879 "parsor_bison.tab.c"
    break;

  case 68: /* expr: expr OR expr  */
#line 390 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>((yyvsp[-2].sv_expression),EXP_OP_OR,(yyvsp[0].sv_expression));
        }
#line 1887 "parsor_bison.tab.c"
    break;

  case 69: /* expr: NOT expr  */
#line 394 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>(std::make_shared<ast::ConstInt>(0),EXP_OP_NOT,(yyvsp[0].sv_expression));
        }
#line 1895 "parsor_bison.tab.c"
    break;

  case 70: /* expr: '-' expr  */
#line 398 "parsor_bison.y"
        {
            (yyval.sv_expression) = std::make_shared<ast::BasicExpr>(std::make_shared<ast::ConstInt>(0),EXP_OP_SUB,(yyvsp[0].sv_expression));
        }
#line 1903 "parsor_bison.tab.c"
    break;

  case 71: /* sys: EXIT  */
#line 404 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::ExitStmt>();
        }
#line 1911 "parsor_bison.tab.c"
    break;

  case 72: /* sys: HELP  */
#line 408 "parsor_bison.y"
        {
            (yyval.sv_node) = std::make_shared<ast::HelpStmt>();
        }
#line 1919 "parsor_bison.tab.c"
    break;


#line 1923 "parsor_bison.tab.c"

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
      yyerror (&yylloc, YY_("syntax error"));
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
                      yytoken, &yylval, &yylloc);
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
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
  yyerror (&yylloc, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 420 "parsor_bison.y"
