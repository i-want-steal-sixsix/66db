#ifndef INTERPRET_H
#define INTERPRET_H

#include<../sqlparser/ast.cpp>
#include<../sm/sm_def.h>
#include<../sm/sm_manager.cpp>
#include<../sm/sm_meta.h>
#include<defs.h>

class Interp {
  public:
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
    static ColType interp_sv_type(ast::SvType sv_type) {
        static std::map<ast::SvType, ColType> m = {
            {ast::SV_TYPE_INT, TYPE_INT}, {ast::SV_TYPE_FLOAT, TYPE_FLOAT}, {ast::SV_TYPE_STRING, TYPE_STRING}};
        return m.at(sv_type);
    }

    static CompOp interp_sv_comp_op(ast::SvCompOp op) {
        static std::map<ast::SvCompOp, CompOp> m = {
            {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
            {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
        };
        return m.at(op);
    }

    static Value interp_sv_value(const std::shared_ptr<ast::Value> &sv_val) {
        Value val;
        if (auto int_lit = std::dynamic_pointer_cast<ast::IntLit>(sv_val)) {
            val.set_int(int_lit->val);
        } else if (auto float_lit = std::dynamic_pointer_cast<ast::FloatLit>(sv_val)) {
            val.set_float(float_lit->val);
        } else if (auto str_lit = std::dynamic_pointer_cast<ast::StringLit>(sv_val)) {
            val.set_str(str_lit->val);
        } else {
            throw InternalError("Unexpected sv value type");
        }
        return val;
    }

    // static std::vector<Condition> interp_where_clause(const std::vector<std::shared_ptr<ast::BinaryExpr>> &sv_conds) {
    //     std::vector<Condition> conds;
    //     for (auto &expr : sv_conds) {
    //         Condition cond;
    //         cond.lhs_col = TabCol(expr->lhs->tab_name, expr->lhs->col_name);
    //         cond.op = interp_sv_comp_op(expr->op);
    //         if (auto rhs_val = std::dynamic_pointer_cast<ast::Value>(expr->rhs)) {
    //             cond.is_rhs_val = true;
    //             cond.rhs_val = interp_sv_value(rhs_val);
    //         } else if (auto rhs_col = std::dynamic_pointer_cast<ast::Col>(expr->rhs)) {
    //             cond.is_rhs_val = false;
    //             cond.rhs_col = TabCol(rhs_col->tab_name, rhs_col->col_name);
    //         }
    //         conds.push_back(cond);
    //     }
    //     return conds;
    // }
;

#endif

