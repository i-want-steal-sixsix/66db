%{
#include <iostream>
#include <memory>
#include "ast.h"
#include "parsor_bison.tab.h"

void yyerror(YYLTYPE *locp, const char* s){
    std::cerr << "Parser Error at LINE " << locp->first_line << " COLUMN " << locp->first_column << ":" << s << std::endl;
}

extern int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

using namespace ast;

%}

// 开启yyerror中的位置参数
%locations
// 无全局/静态变量，可重用parser
%define api.pure full
// enable verbose syntax error message
//%define parse.error verbose

// 关键词
%token EXIT HELP
%token CREATE TABLE DROP SHOW DATABASE USE
%token INT FLOAT CHAR
%token PRIMARY KEY sNULL
%token SELECT FROM WHERE INSERT INTO VALUES DELETE UPDATE SET AS
%token AND OR NOT

// 多字符运算符
%token NEQ LE GE

// 类型
%type <sv_node> stmt dml ddl sys
%type <sv_string> tabName colName altName opt_as dbName
%type <sv_fromtab> fromTab
%type <sv_fromtabs> tabList
%type <sv_column> col
%type <sv_colnames> colNameList opt_colNameList
//%type <sv_columns> colList
%type <sv_selcol> selcol
%type <sv_selcols> selcolList selList
%type <sv_fieldoption> option optionList opt_Option
%type <sv_fieldinfo> field
%type <sv_fieldinfos> fieldList
%type <sv_datatype> dataType
%type <sv_value> value
%type <sv_values> valueList
%type <sv_expression> expr opt_where
%type <sv_updexpr> upd
%type <sv_updexprs> updList

// 常量及标识符
%token CONST_INT CONST_STRING CONST_FLOAT
%token IDENTIFIER

// 类型
%type<sv_int> CONST_INT
%type<sv_float> CONST_FLOAT
%type<sv_string> CONST_STRING IDENTIFIER

//优先级
%left OR
%left AND
%left NOT
%left '<' '>' '=' NEQ LE GE
%left '+' '-'
%left '*' '/'
%left UMINUS

%%

menu:           stmt ';'
        {
            ast::parse_tree = $1;
            YYACCEPT;
        }
        ;

stmt:           sys
        {}
        |       dml
        {}
        |       ddl
        {}
        ;

value:          CONST_INT
        {
            $$ = std::make_shared<ConstInt>($1);
        }
        |       CONST_FLOAT
        {
            $$ = std::make_shared<ConstFloat>($1);
        }
        |       CONST_STRING
        {
            $$ = std::make_shared<ConstString>($1);
        }
        ;

valueList:      value
        {
            $$ = std::vector<std::shared_ptr<ast::ConstValue>>{$1};
        }
        |       valueList ',' value
        {
            $$.push_back($3);
        }


colNameList:    colName
        {
            $$ = std::vector<std::string>{$1};
        }
        |       colNameList ',' colName
        {
            $$.push_back($3);
        }
        ;

opt_colNameList: /* 缺省 */
        {
            $$ = {};
        }
        |       '(' colNameList ')'
        {
            $$ = $2;
        }
        ;

col:            tabName '.' colName
        {
            $$ = std::make_shared<ast::Column>($1, $3);
        }
        |       colName
        {
            $$ = std::make_shared<ast::Column>("", $1);
        }
        ;

opt_as:         /* 缺省 */
        {
            $$ = "";
        }
        |       AS altName
        {
            $$ = $2;
        }
        |       altName
        {
            $$ = $1; 
        }
/*
colList:        col
        {
            $$ = std::vector<std::shared_ptr<ast::Column>>{$1};
        }
        |       colList ',' col
        {
            $$.push_back($3);
        }
        ;
*/
selcol:         col opt_as
        {
            $$ = std::make_shared<ast::SelColumn>($1,$2);
        }
        ;

selcolList:     selcol
        {
            $$ = std::vector<std::shared_ptr<ast::SelColumn>>{$1};
        }
        |       selcolList ',' selcol
        {
            $$.push_back($3);
        }
        ;

fromTab:        tabName AS altName
        {
            $$ = std::make_shared<ast::FromTable>($1,$3);
        }
        |       tabName altName
        {
            $$ = std::make_shared<ast::FromTable>($1,$2);
        }
        |       tabName
        {
            $$ = std::make_shared<ast::FromTable>($1,"");
        }
        ;

tabList:        fromTab
        {
            $$ = std::vector<std::shared_ptr<FromTable>>{$1};
        }
        |       tabList ',' fromTab
        {
            $$.push_back($3);
        }
        ;

dataType:       INT
        {
            $$ = {FIELD_INT,4};
        }
        |       FLOAT
        {
            $$ = {FIELD_FLOAT,4};
        }
        |       CHAR '(' CONST_INT ')'
        {
            $$ = {FIELD_CHAR,$3};
        }
        ;

option:         NOT sNULL
        {
            $$ = FIELD_OPT_NOTNULL;
        }
        |       PRIMARY KEY
        {
            $$ = FIELD_OPT_PRIMKEY;
        }
        ;

optionList:     option
        {
            $$ = $1;
        }
        |       optionList option
        {
            $$ = ($1 | $2);
        }
        ;

opt_Option:      /* 缺省 */ 
        {
            $$ = 0;
        }
        |       optionList
        {
            $$ = $1;
        }
        ;

field:          colName dataType opt_Option
        {
            $$ = std::make_shared<ast::FieldInfo>($1,$2,$3);
        }
        ;

fieldList:      field
        {
            $$ = std::vector<std::shared_ptr<ast::FieldInfo>>{$1};
        }
        |       fieldList ',' field
        {
            $$.push_back($3);
        }
        ;

upd:            col '=' expr
        {
            $$ = std::make_shared<ast::UpdateExpr>($1,$3);
        }
        ;

updList:        upd
        {
            $$ = std::vector<std::shared_ptr<ast::UpdateExpr>>{$1};
        }
        |       updList ',' upd
        {
            $$.push_back($3);
        }
        ;

dml:            SELECT selList FROM tabList opt_where
        {
            $$ = std::make_shared<ast::SelectStmt>($2, $4, $5);
        }
        |       INSERT INTO tabName opt_colNameList VALUES '(' valueList ')'
        {
            $$ = std::make_shared<ast::InsertStmt>($3,$4,$7);
        }
        |       DELETE FROM tabName opt_where
        {
            $$ = std::make_shared<ast::DeleteStmt>($3,$4);
        }
        |       UPDATE tabName SET updList opt_where
        {
            $$ = std::make_shared<ast::UpdateStmt>($2,$4,$5);
        }
        ;

ddl:            CREATE TABLE tabName '(' fieldList ')'
        {
            $$ = std::make_shared<ast::CreateTableStmt>($3, $5);
        }
        |       DROP TABLE tabName
        {
            $$ = std::make_shared<ast::DropTableStmt>($3);
        }
        |       SHOW TABLE
        {
            $$ = std::make_shared<ast::ShowTableStmt>();
        }
        |       CREATE DATABASE dbName
        {
            $$ = std::make_shared<ast::CreateDBStmt>($3);
        }
        |       SHOW DATABASE
        {
            $$ = std::make_shared<ast::ShowDBStmt>();
        }
        |       USE DATABASE dbName
        {
            $$ = std::make_shared<ast::OpenDBStmt>($3);
        }
        |       DROP DATABASE dbName
        {
            $$ = std::make_shared<ast::DropDBStmt>($3);
        }
        ;

selList:        '*'
        {
            $$ = {};
        }
        |       selcolList
        ;

opt_where:      /* 缺省 */ 
        { 
            $$ = std::make_shared<ConstInt>(1);
        }
        |       WHERE expr
        {
            $$ = $2;
        }
        ;

expr:           value
        {
            $$ = $1;
        }
        |
                col
        {
            $$ = $1;
        }
        |       '(' expr ')'
        {
            $$ = $2;
        }
        |       expr '+' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_ADD,$3);
        }
        |       expr '-' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_SUB,$3);
        }
        |       expr '*' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_MUL,$3);
        }
        |       expr '/' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_DIV,$3);
        }
        |       expr '=' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_EQU,$3);
        }
        |       expr NEQ expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_NEQ,$3);
        }
        |       expr '<' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_L,$3);
        }
        |       expr LE expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_LE,$3);
        }
        |       expr '>' expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_G,$3);
        }
        |       expr GE expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_GE,$3);
        }
        |       expr AND expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_AND,$3);
        }
        |       expr OR expr
        {
            $$ = std::make_shared<ast::BasicExpr>($1,EXP_OP_OR,$3);
        }
        |       NOT expr
        {
            $$ = std::make_shared<ast::BasicExpr>(std::make_shared<ast::ConstInt>(0),EXP_OP_NOT,$2);
        }
        |       '-' expr %prec UMINUS
        {
            $$ = std::make_shared<ast::BasicExpr>(std::make_shared<ast::ConstInt>(0),EXP_OP_SUB,$2);
        }
        ;

sys:            EXIT
        {
            $$ = std::make_shared<ast::ExitStmt>();
        }
        |       HELP
        {
            $$ = std::make_shared<ast::HelpStmt>();
        }
        ;



tabName: IDENTIFIER;
colName: IDENTIFIER;
altName: IDENTIFIER;
dbName: IDENTIFIER;


%%