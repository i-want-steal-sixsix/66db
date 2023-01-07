/* 大小写不敏感 */
%option caseless            

%option noyywrap

%option nounput

%option noinput

%option bison-bridge
%option bison-locations

/* 生成头文件 */
%option header-file="lex.yy.h"

%{

#include "ast.h"
#include "parsor_bison.tab.h"

// 自动更新位置
#define YY_USER_ACTION \
yylloc->first_line = yylloc->last_line; \
yylloc->first_column = yylloc->last_column; \
for(int i = 0; yytext[i] != '\0'; i++){ \
    if(yytext[i] == '\n') { \
        yylloc->last_line++; \
        yylloc->last_column = 1; \
    } else { \
        yylloc->last_column++; \
    } \
}
extern int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

%}

alpha           [a-zA-Z]
digit           [0-9]

identifier      {alpha}(_|{alpha}|{digit})*
const_int       {digit}+
const_float     {const_int}+\.({digit}+)?
const_string    '[^']*'

separator       ";"|","|"."
single_op       "+"|"-"|"*"|"/"|"="|"<"|">"|"("|")"

%%

"--".*          {/*忽略单行注释后的内容*/}

"EXIT"          {return EXIT;}
"HELP"          {return HELP;}

"CREATE"        {return CREATE;}
"TABLE"         {return TABLE;}
"INT"           {return INT;}
"FLOAT"         {return FLOAT;}
"CHAR"          {return CHAR;}
"PRIMARY"       {return PRIMARY;}
"KEY"           {return KEY;}
"NOT"           {return NOT;}
"NULL"          {return sNULL;}
"DROP"          {return DROP;}
"SHOW"          {return SHOW;}

"SELECT"        {return SELECT;}
"FROM"          {return FROM;}
"WHERE"         {return WHERE;}
"AS"            {return AS;}

"INSERT"        {return INSERT;}
"INTO"          {return INTO;}
"VALUES"        {return VALUES;}

"DELETE"        {return DELETE;}

"UPDATE"        {return UPDATE;}
"SET"           {return SET;}

"AND"           {return AND;}
"OR"            {return OR;}

"<>"            {return NEQ;}
"<="            {return LE;}
">="            {return GE;}

{identifier}    {yylval->sv_string = yytext;
                 return IDENTIFIER;}

{const_int}     {yylval->sv_int = atoi(yytext);
                 return CONST_INT;}

{const_float}   {yylval->sv_float = atof(yytext);
                 return CONST_FLOAT;}

{const_string}  {yylval->sv_string = std::string(yytext + 1, strlen(yytext) - 2);
                 return CONST_STRING;}

{separator}|{single_op}     {return yytext[0];}

"\n"            {}
.               {}

%%