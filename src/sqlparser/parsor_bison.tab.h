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
    DATABASE = 264,                /* DATABASE  */
    USE = 265,                     /* USE  */
    INT = 266,                     /* INT  */
    FLOAT = 267,                   /* FLOAT  */
    CHAR = 268,                    /* CHAR  */
    PRIMARY = 269,                 /* PRIMARY  */
    KEY = 270,                     /* KEY  */
    sNULL = 271,                   /* sNULL  */
    SELECT = 272,                  /* SELECT  */
    FROM = 273,                    /* FROM  */
    WHERE = 274,                   /* WHERE  */
    INSERT = 275,                  /* INSERT  */
    INTO = 276,                    /* INTO  */
    VALUES = 277,                  /* VALUES  */
    DELETE = 278,                  /* DELETE  */
    UPDATE = 279,                  /* UPDATE  */
    SET = 280,                     /* SET  */
    AS = 281,                      /* AS  */
    AND = 282,                     /* AND  */
    OR = 283,                      /* OR  */
    NOT = 284,                     /* NOT  */
    NEQ = 285,                     /* NEQ  */
    LE = 286,                      /* LE  */
    GE = 287,                      /* GE  */
    CONST_INT = 288,               /* CONST_INT  */
    CONST_STRING = 289,            /* CONST_STRING  */
    CONST_FLOAT = 290,             /* CONST_FLOAT  */
    IDENTIFIER = 291,              /* IDENTIFIER  */
    UMINUS = 292                   /* UMINUS  */
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
