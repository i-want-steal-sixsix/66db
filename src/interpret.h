#ifndef INTERPRET_H
#define INTERPRET_H

#include<../sqlparser/ast.cpp>
#include<../sm/sm_def.h>
#include<../sm/sm_manager.cpp>
#include<../sm/sm_meta.h>
#include<defs.h>

class Interp {
  public:
    static void interp_sql(const std::shared_ptr<ast::ASTNode> &root) {
        if (auto x = std::dynamic_pointer_cast<ast::HelpStmt>(root)) {
            std::cout
                << "Supported SQL syntax:\n"
                   "  command ;\n"
                   "command:\n"
                   "  CREATE TABLE table_name (column_name type [, column_name type ...])\n"
                   "  DROP TABLE table_name\n"
                   "  CREATE INDEX table_name (column_name)\n"
                   "  DROP INDEX table_name (column_name)\n"
                   "  INSERT INTO table_name VALUES (value [, value ...])\n"
                   "  DELETE FROM table_name [WHERE where_clause]\n"
                   "  UPDATE table_name SET column_name = value [, column_name = value ...] [WHERE where_clause]\n"
                   "  SELECT selector FROM table_name [WHERE where_clause]\n"
                   "type:\n"
                   "  {INT | FLOAT | CHAR(n)}\n"
                   "where_clause:\n"
                   "  condition [AND condition ...]\n"
                   "condition:\n"
                   "  column op {column | value}\n"
                   "column:\n"
                   "  [table_name.]column_name\n"
                   "op:\n"
                   "  {= | <> | < | > | <= | >=}\n"
                   "selector:\n"
                   "  {* | column [, column ...]}\n";

            //sm
        } else if (auto x = std::dynamic_pointer_cast<ast::ShowTableStmt>(root)) {
            SmManager::show_tables();
        } else if (auto x = std::dynamic_pointer_cast<ast::CreateTableStmt>(root)) {//pr
            std::vector<ColDef> col_defs;
            std::vector<std::shared_ptr<ast::FieldInfo>> tmp=x->fields;
            std::vector<std::shared_ptr<ast::FieldInfo>>::iterator i;
            for(i=tmp.begin();i<tmp.end();i++){
                std::string _name = i->get()->name;
                uint8_t _type = (uint8_t)(i->get()->datatype.type);
                uint16_t _len = (uint16_t)(i->get()->datatype.length);
                uint8_t _option = (uint8_t)(i->get()->option);
                ColDef coldef(_name,_type,_len,_option);
                col_defs.push_back(coldef);
            }
            SmManager::create_table(x->name, col_defs);
        } else if (auto x = std::dynamic_pointer_cast<ast::DropTableStmt>(root)) {
            SmManager::drop_table(x->name);

            //ql
        } else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(root)) {
            std::vector<Value> values;
            for (auto &sv_val : x->vals) {
                values.push_back(interp_sv_value(sv_val));
            }
            QlManager::insert_into(x->tab_name, values);
        } else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(root)) {
            std::vector<Condition> conds = interp_where_clause(x->conds);
            QlManager::delete_from(x->tab_name, conds);
        } else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(root)) {
            std::vector<Condition> conds = interp_where_clause(x->conds);
            std::vector<SetClause> set_clauses;
            for (auto &sv_set_clause : x->set_clauses) {
                SetClause set_clause(TabCol("", sv_set_clause->col_name), interp_sv_value(sv_set_clause->val));
                set_clauses.push_back(set_clause);
            }
            QlManager::update_set(x->tab_name, set_clauses, conds);
        } else if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(root)) {
            inter_w vecs = interp_where(x->condition);            
            // std::vector<TabCol> sel_cols;
            // for (auto &sv_sel_col : x->cols) {
            //     TabCol sel_col(sv_sel_col->tab_name, sv_sel_col->col_name);
            //     sel_cols.push_back(sel_col);
            // }
            QlManager::select_from(sel_cols, x->tabs, vecs);
        } else {
            throw InternalError("Unexpected AST root");
        }
    }

  private:

  //ast2defs
    static ColType interp_DataType(ast::DataType dt) {
        static std::map<ast::DataType, ColType> m = {
            {ast::FIELD_INT, COL_TYPE_INT}, {ast::FIELD_FLOAT, COL_TYPE_FLOAT}, {ast::FIELD_CHAR, COL_TYPE_CHAR}};
        return m.at(dt);
    }

    static CompOp interp_sv_comp_op(ast::SvCompOp op) {
        static std::map<ast::SvCompOp, CompOp> m = {
            {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
            {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
        };
        return m.at(op);
    }
//ast2ql
    static Value interp_sv_value(const std::shared_ptr<ast::ConstValue> &cst_val) {
        Value val;//need ql
        if (auto int_ = std::dynamic_pointer_cast<ast::ConstInt>(cst_val)) {
            val.set_int(int_->value);
        } else if (auto float_ = std::dynamic_pointer_cast<ast::ConstFloat>(cst_val)) {
            val.set_float(float_->value);
        } else if (auto str_ = std::dynamic_pointer_cast<ast::ConstString>(cst_val)) {
            val.set_str(str_->value);
        } else {
            throw InternalError("Unexpected sv value type");
        }
        return val;
    }

    struct inter_w{
        std::vector< std::string > vec1;
        std::vector< int > vec2;
        inter_w(std::vector< std::string > in_vec1,std::vector< int > in_vec2):
        vec1(in_vec1),vec2(in_vec2){}
    };
//for where
    static inter_w interp_where(const std::shared_ptr<ast::Expression> &root){
        std::vector< std::string > v1;// 按顺序存放节点数据
        std::vector< int > v2;//按顺序存放节点类型

    if(auto x = std::dynamic_pointer_cast<ast::ConstInt>(root)){
        v1.push_back(std::to_string(x->value));
        v2.push_back(str2int["int"]);
    }
    else if(auto x = std::dynamic_pointer_cast<ast::ConstFloat>(root)){
        v1.push_back(std::to_string(x->value));
        v2.push_back(str2int["float"]);
    }
    else if(auto x = std::dynamic_pointer_cast<ast::ConstString>(root)){
        v1.push_back(x->value);
        v2.push_back(str2int["string"]);
    }
    else if (auto x = std::dynamic_pointer_cast<ast::BasicExpr>(root)){
       interp_where(x->lExpr);
       v1.push_back(opStr[x->op]);
       v2.push_back(str2int["ExprOps"]);
       interp_where(x->rExpr);
    }
    else{
        throw WindowsError();
    }
    
    inter_w Vec(v1,v2);
    return Vec;
    }
};
#endif
