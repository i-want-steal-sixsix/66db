/************************
 * Abstract Syntax Tree *
 ************************/

#ifndef SQL_PARSER_AST
#define SQL_PARSER_AST

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace ast{

// 运算符
enum ExprOps{
    EXP_OP_ADD,EXP_OP_SUB,EXP_OP_MUL,EXP_OP_DIV,
    EXP_OP_EQU,EXP_OP_NEQ,
    EXP_OP_L  ,EXP_OP_LE ,EXP_OP_G  ,EXP_OP_GE ,
    EXP_OP_AND,EXP_OP_OR ,EXP_OP_NOT
};

// 域
enum DataType{
    FIELD_INT,FIELD_FLOAT,FIELD_CHAR
};

// 域选项
enum FieldOption{
    FIELD_OPT_NOTNULL = 0x01,
    FIELD_OPT_PRIMKEY = 0x02,
};

// 数据类型信息
struct DataInfo{
    int type;
    int length;
};

// 域信息
struct FieldInfo{
    std::string name;
    DataInfo datatype;
    int8_t option;

    FieldInfo(std::string IN_name, DataInfo IN_datatype, int8_t IN_option)
    :name(std::move(IN_name)),datatype(IN_datatype),option(IN_option){}

    // Debug Function
    void debug_print(){
        std::cout << name << " ";
        switch(datatype.type){
            case FIELD_INT:
                std::cout << "INT ";
                break;
            case FIELD_FLOAT:
                std::cout << "FLOAT ";
                break;
            case FIELD_CHAR:
                std::cout << "CHAR(" << datatype.length << ") ";
                break;
            default:
                std::cout << "WRONG TYPE ";
        }
        if(option & FIELD_OPT_NOTNULL){
            std::cout << "NOT NULL ";
        }
        if(option & FIELD_OPT_PRIMKEY){
            std::cout << "PRIMARY KEY ";
        }
        std::cout << std::endl;
    }

};

// 抽象节点
struct ASTNode{

    virtual void debug_print(){
        std::cout << "(empty node)" << std::endl;
    };
};

// 抽象表达式
struct Expression : public ASTNode{};

// 抽象常量
struct ConstValue : public Expression{};

// 列
struct Column : public Expression{
    std::string colName;
    std::string tabName;

    Column(std::string IN_colName, std::string IN_tabName)
    :colName(std::move(IN_colName)),tabName(std::move(IN_tabName)){}

    // Debug Function
    void debug_print(){
        if(tabName == "")
            std::cout << "(NULL)" << "." << colName;
        else
            std::cout << tabName << "." << colName;
    }

};

// SELECT 列（含有别名）
struct SelColumn : public ASTNode{
    std::shared_ptr<Column> column;
    std::string altName;

    SelColumn(std::shared_ptr<Column> IN_column, std::string IN_altName)
    :column(std::move(IN_column)),altName(std::move(IN_altName)){}

    //Debug
    void debug_print(){
        column.get()->debug_print();
        if(altName[0] != '~'){
            std::cout << " <AS> ";
            std::cout << altName;
        }
    }

};

// FROM 表（含有别名）
struct FromTable : public ASTNode{
    std::string tabName;
    std::string altName;

    FromTable(std::string IN_tabName, std::string IN_altName)
    :tabName(std::move(IN_tabName)),altName(std::move(IN_altName)){}

    //Debug
    void debug_print(){
        std::cout << tabName;
        if(altName[0] != '~'){
            std::cout << " <AS> ";
            std::cout << altName;
        }
    }

};

// 常量节点
struct ConstInt : public ConstValue{
    int value;

    ConstInt(int IN_value):value(IN_value){}

    //Debug
    void debug_print(){std::cout<<value;}

};

struct ConstFloat : public ConstValue{
    float value;

    ConstFloat(float IN_value):value(IN_value){}

    //Debug
    void debug_print(){std::cout<<value;}

};

struct ConstString : public ConstValue{
    std::string value;

    ConstString(std::string IN_value):value(std::move(IN_value)){}

    //Debug
    void debug_print(){std::cout<<'\''<<value<<'\'';}

};

// 基本表达式
struct BasicExpr : public Expression{

    std::shared_ptr<Expression> lExpr;
    ExprOps op;
    std::shared_ptr<Expression> rExpr;
    
    BasicExpr(std::shared_ptr<Expression> IN_lExpr, ExprOps IN_op, std::shared_ptr<Expression> IN_rExpr)
    :lExpr(std::move(IN_lExpr)), op(IN_op), rExpr(std::move(IN_rExpr)){}

    //Debug
    void debug_print(){
        std::cout << "(";
        if(op != EXP_OP_NOT){
            lExpr.get()->debug_print();
        }
        //What a bad implementation...
        std::string opStr[] = { " + " ," - " ," * " ," / " ,
                                " = " ," <> ",
                                " < " ," <= "," > " ," >= ",
                                " && "," || ", "~"};
        std::cout << opStr[op];
        if(rExpr != NULL){
            rExpr.get()->debug_print();
        }
        std::cout << ")";
    }

};

// 更新表达式
struct UpdateExpr : public Expression{

    std::shared_ptr<Column> column;
    std::shared_ptr<Expression> expression;

    UpdateExpr(std::shared_ptr<Column> IN_column, std::shared_ptr<Expression> IN_expression)
    :column(std::move(IN_column)), expression(std::move(IN_expression)){}

    //Debug
    void debug_print(){
        column.get()->debug_print();
        std::cout << " = ";
        expression.get()->debug_print();
        std::cout << std::endl;
    }

};

// 创建表语句
struct CreateTableStmt : public ASTNode {
    std::string name;
    std::vector<std::shared_ptr<FieldInfo>> fields;

    CreateTableStmt(std::string IN_name, std::vector<std::shared_ptr<FieldInfo>> IN_fields)
    :name(std::move(IN_name)),fields(std::move(IN_fields)){}

    // Debug Function
    void debug_print(){
        std::cout << "<CREATE TABLE> " << name << std::endl;
        std::string typeStr[] = {"INT", "FLOAT", "CHAR"};
        for(int i = 0; i < fields.size(); i++){
            fields[i].get()->debug_print();
        }
    }
};

// 删除表语句
struct DropTableStmt : public ASTNode {
    std::string name;

    DropTableStmt(std::string IN_name)
    :name(std::move(IN_name)){}

    // Debug Function
    void debug_print(){
        std::cout << "<DROP TABLE> " << name << std::endl;
    }
};

// 展示所有表语句
struct ShowTableStmt : public ASTNode{

    ShowTableStmt(){}

    // Debug Function
    void debug_print(){
        std::cout << "<SHOW TABLE> " << std::endl;
    }
};

// 选择语句
struct SelectStmt : public ASTNode {
    std::vector<std::shared_ptr<SelColumn>> columns;
    //FROM
    std::vector<std::shared_ptr<FromTable>> tables;
    //WHERE
    std::shared_ptr<Expression> condition;

    SelectStmt(std::vector<std::shared_ptr<SelColumn>> IN_columns, std::vector<std::shared_ptr<FromTable>> IN_tables, std::shared_ptr<Expression> IN_condition)
    :columns(std::move(IN_columns)),tables(std::move(IN_tables)),condition(std::move(IN_condition)){}

    // Debug Function
    void debug_print(){
        std::cout << "<SELECT> ";
        if(columns.size() == 0){
            std::cout << "(ALL)";
        }
        else{
            columns[0].get()->debug_print();
            for(int i = 1; i < columns.size(); i++){
                std::cout << ",";
                columns[i].get()->debug_print();
            }
        }
        std::cout << std::endl;
        std::cout << "<FROM> ";
        tables[0].get()->debug_print();
        for(int i = 1; i < tables.size(); i++){
            std::cout << ",";
            tables[i].get()->debug_print();
        }
        std::cout << std::endl;
        std::cout << "<WHERE> ";
        condition.get()->debug_print();
        std::cout << std::endl;
    }

};

// 插入语句
struct InsertStmt : public ASTNode {
    std::string tableName;
    //指定列
    std::vector<std::string> columns;
    //VALUES
    std::vector<std::shared_ptr<ConstValue>> values;

    InsertStmt(std::string IN_tableName, std::vector<std::string> IN_columns, std::vector<std::shared_ptr<ConstValue>> IN_values)
    :tableName(std::move(IN_tableName)),columns(std::move(IN_columns)),values(std::move(IN_values)){}

    // Debug Function
    void debug_print(){
        std::cout << "<INSERT INTO> " << tableName << std::endl;
        std::cout << "<COLUMN> ";
        if(columns.size() == 0){
            std::cout << "(ALL)";
        }
        else{
            for(int i = 0; i < columns.size(); i++){
                std::cout << columns[i] << " ";
            }
        }
        std::cout << std::endl;
        std::cout << "<VALUES> ";
        for(int i = 0; i < values.size(); i++){
            values[i].get()->debug_print();
            std::cout << " ";
        }
        std::cout << std::endl;
    }

};

// 删除语句
struct DeleteStmt : public ASTNode {
    std::string table;
    //WHERE
    std::shared_ptr<Expression> condition;

    DeleteStmt(std::string IN_table, std::shared_ptr<Expression> IN_condition)
    :table(std::move(IN_table)),condition(std::move(IN_condition)){}

    // Debug Function
    void debug_print(){
        std::cout << "<DELETE FROM> " << table << std::endl;
        std::cout << "<WHERE> ";
        condition.get()->debug_print();
        std::cout << std::endl;
    }

};

// 更新语句
struct UpdateStmt : public ASTNode {
    std::string table;
    //SET
    std::vector<std::shared_ptr<UpdateExpr>> updList;
    //WHERE
    std::shared_ptr<Expression> condition;

    UpdateStmt(std::string IN_table, std::vector<std::shared_ptr<UpdateExpr>> IN_updList, std::shared_ptr<Expression> IN_condition)
    :table(std::move(IN_table)),updList(std::move(IN_updList)),condition(std::move(IN_condition)){}

    // Debug Function
    void debug_print(){
        std::cout << "<UPDATE> " << table << std::endl;
        for(int i = 0; i < updList.size(); i++){
            updList[i].get()->debug_print();
        }
        std::cout << "<WHERE> ";
        condition.get()->debug_print();
        std::cout << std::endl;
    }

};

// 系统语句

struct ExitStmt : public ASTNode{

    // Debug Function
    void debug_print(){
        std::cout << "<EXIT> " << std::endl;
    }
};

struct HelpStmt : public ASTNode{

    // Debug Function
    void debug_print(){
        std::cout << "<HELP> " << std::endl;
    }
};

// 语义值
struct SVSet {
    // 常量
    int sv_int;
    float sv_float;
    std::string sv_string;
    std::vector<std::string> sv_strings;
    // 抽象常量
    std::shared_ptr<ConstValue> sv_value;
    std::vector<std::shared_ptr<ConstValue>> sv_values;
    // 树节点
    std::shared_ptr<ASTNode> sv_node;
    // 域类型
    DataInfo sv_datatype;
    std::shared_ptr<FieldInfo> sv_fieldinfo;
    std::vector<std::shared_ptr<FieldInfo>> sv_fieldinfos;
    // 域约束
    int8_t sv_fieldoption;
    // 列
    std::vector<std::string> sv_colnames;
    std::shared_ptr<Column> sv_column;
    std::vector<std::shared_ptr<Column>> sv_columns;
    std::shared_ptr<SelColumn> sv_selcol;
    std::vector<std::shared_ptr<SelColumn>> sv_selcols;
    // 表
    std::shared_ptr<FromTable> sv_fromtab;
    std::vector<std::shared_ptr<FromTable>> sv_fromtabs;
    // 运算符
    ExprOps sv_op;
    // 表达式
    std::shared_ptr<Expression> sv_expression;
    // 更新表达式
    std::shared_ptr<UpdateExpr> sv_updexpr;
    std::vector<std::shared_ptr<UpdateExpr>> sv_updexprs;

};

#define YYSTYPE ast::SVSet

// 记录位置
struct location{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};

#define YYLTYPE ast::location

extern std::shared_ptr<ast::ASTNode> parse_tree;

}

#endif