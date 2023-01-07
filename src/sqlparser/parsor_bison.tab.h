/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSOR_BISON_TAB_H_INCLUDED
# define YY_YY_PARSOR_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    EXIT = 258,                    /* EXIT  */
    HELP = 259,                    /* HELP  */
    CREATE = 260,                  /* CREATE  */
    TABLE = 261,                   /* TABLE  */
    DROP = 262,                    /* DROP  */
    SHOW = 263,                    /* SHOW  */
    INT = 264,                     /* INT  */
    FLOAT = 265,                   /* FLOAT  */
    CHAR = 266,                    /* CHAR  */
    PRIMARY = 267,                 /* PRIMARY  */
    KEY = 268,                     /* KEY  */
    sNULL = 269,                   /* sNULL  */
    SELECT = 270,                  /* SELECT  */
    FROM = 271,                    /* FROM  */
    WHERE = 272,                   /* WHERE  */
    INSERT = 273,                  /* INSERT  */
    INTO = 274,                    /* INTO  */
    VALUES = 275,                  /* VALUES  */
    DELETE = 276,                  /* DELETE  */
    UPDATE = 277,                  /* UPDATE  */
    SET = 278,                     /* SET  */
    AS = 279,                      /* AS  */
    AND = 280,                     /* AND  */
    OR = 281,                      /* OR  */
    NOT = 282,                     /* NOT  */
    NEQ = 283,                     /* NEQ  */
    LE = 284,                      /* LE  */
    GE = 285,                      /* GE  */
    CONST_INT = 286,               /* CONST_INT  */
    CONST_STRING = 287,            /* CONST_STRING  */
    CONST_FLOAT = 288,             /* CONST_FLOAT  */
    IDENTIFIER = 289,              /* IDENTIFIER  */
    UMINUS = 290                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (void);


#endif /* !YY_YY_PARSOR_BISON_TAB_H_INCLUDED  */
