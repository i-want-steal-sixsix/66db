#ifndef INTERPRET_H
#define INTERPRET_H

#include "./sqlparser/ast.h"
#include "./sm/sm_def.h"
#include "./sm/sm_manager.h"
#include "./sm/sm_meta.h"
#include "./ql/ql_manager.h"
#include "defs.h"

class Interp {
public:

    static std::map<std::string,std::string> _alt2tab;

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
            std::string _tab_name = x->name;
            std::vector<std::string> _col_names;
            for(i=tmp.begin();i<tmp.end();i++){
                std::string _col_name = i->get()->name;
                uint8_t _type = (uint8_t)(i->get()->datatype.type);
                uint16_t _len = (uint16_t)(i->get()->datatype.length);
                uint8_t _option = (uint8_t)(i->get()->option);
                //_col_names
                _col_names.push_back(_col_name);
                //coldef
                ColDef coldef(_col_name,_type,_len,_option);
                col_defs.push_back(coldef);
            }
            SmManager::create_table(_tab_name, col_defs);
        } else if (auto x = std::dynamic_pointer_cast<ast::DropTableStmt>(root)) {
            SmManager::drop_table(x->name);

            //ql
        }

        
        // else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(root)) {
        //     std::vector<Value> values;
        //     for (auto &sv_val : x->vals) {
        //         values.push_back(interp_sv_value(sv_val));
        //     }
        //     QlManager::insert_into(x->tab_name, values);
        // } else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(root)) {
        //     std::vector<Condition> conds = interp_where_clause(x->conds);
        //     QlManager::delete_from(x->tab_name, conds);


        // } else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(root)) {
        //     std::vector<Condition> conds = interp_where_clause(x->conds);
        //     std::vector<SetClause> set_clauses;
        //     for (auto &sv_set_clause : x->set_clauses) {
        //         SetClause set_clause(TabCol("", sv_set_clause->col_name), interp_sv_value(sv_set_clause->val));
        //         set_clauses.push_back(set_clause);
        //     }
        //     QlManager::update_set(x->tab_name, set_clauses, conds);

        //     } 
            else if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(root)) {
            std::cout<<"sel"<<std::endl;
            //tab_names
            std::map<std::string,std::string> _used_tab2alt;
            std::vector<std::string> _used_table;
            std::vector<SelTabMeta> tab_names;
            std::vector<std::shared_ptr<ast::FromTable>> _tables = x->tables;
            std::vector<std::shared_ptr<ast::FromTable>>::iterator it;
            for(it=_tables.begin();it<_tables.end();it++){
                std::string _table_name = it->get()->tabName;
                std::string _alt_name;
                if(!it->get()->altName.empty()){
                    _alt_name = it->get()->altName;
                }
                else{
                    _alt_name = _table_name;
                }
                SelTabMeta sel_tab(_alt_name, _table_name);
                tab_names.push_back(sel_tab);
                //alt2tab
                if(_alt2tab.find(_alt_name) == _alt2tab.end()){
                    _alt2tab[_alt_name] = _table_name;
                }
                else{
                    std::cout<<"repeated alt_name";
                    return;
                }
                //_used_table
                _used_table.push_back(_alt_name);
                //_used_tab2alt
                _used_tab2alt[_table_name] = _alt_name;
            }
            std::cout<<"tab'ok"<<std::endl;
            //conds
            std::vector< std::string > v1;      // 按顺序存放节点数据
            std::vector< int > v2;              // 按顺序存放节点类型
            interp_where(x->condition, _used_table,_used_tab2alt, v1, v2);
            inter_w conds(v1,v2);
            std::cout<<"conds'ok"<<std::endl;
            //sel_cols
            std::vector<SelColMeta> sel_cols;       
            std::vector<std::shared_ptr<ast::SelColumn>> _columns = x->columns;
            std::vector<std::shared_ptr<ast::SelColumn>>::iterator ic;

            std::string _table_name;
            std::string _col_name;
            std::string _alt_name;

            for(ic=_columns.begin();ic<_columns.end();ic++){
                _table_name = ic->get()->column->tabName;
                _col_name = ic->get()->column->colName;
                _alt_name = ic->get()->altName;
                // 
                if(_col_name.empty()){
                    std::cout<<"no_col_error"<<std::endl;
                    return;
                }

                if(_table_name.empty()){
                    bool tab_flag = false;
                    for(int i=0;i<_used_table.size();i++){
                        if(SmManager::db.get_table(_alt2tab[_used_table[i]]).is_col(_col_name)){
                            if(tab_flag){
                                std::cout << "multiple possibility for column: " << _col_name << std::endl;
                                return;
                            }
                            _table_name = _used_table[i];
                            tab_flag = true;
                        }
                    }
                }
                if(_table_name.empty()){
                    std::cout<<"no such column"<<std::endl;
                    return;
                }
                
                SelColMeta sel_col(_table_name,_col_name,_alt_name);
                sel_cols.push_back(sel_col);
            }

            std::cout << "sel_cols:\n";
            for(auto &x : sel_cols){
                std::cout << x.tab_name << "  " << x.col_name << "  " << x.show_name << std::endl;
            }
            std::cout << "\nsel_tabs:\n";
            for(auto &x : tab_names){
                std::cout << x.tab_name << "  " << x.alt_name << std::endl;
            }
            std::cout << "\nwhere_clause:\n";
            std::cout << "value / OP:\n";
            for(auto &x : conds.vec1){
                std::cout << x << "    ";
            }
            std::cout << "\ntype:\n";
            for(auto &x : conds.vec2){
                std::cout << x << "    ";
            }



            QlManager::select_from(sel_cols, tab_names);
            _alt2tab.clear();
            //QlManager::select_from(sel_cols, x->tabs, conds);
        }
        




        else {
            throw DBError("Unexpected AST root");
        }

        

    }

  private:

  

    //_ast2defs_type
    static ColType interp_DataType(ast::DataType dt) {
        static std::map<ast::DataType, ColType> _ast2defs_type = {
            {ast::FIELD_INT, COL_TYPE_INT}, {ast::FIELD_FLOAT, COL_TYPE_FLOAT}, {ast::FIELD_CHAR, COL_TYPE_CHAR}};
        return _ast2defs_type.at(dt);
    }

    /* ql里没有定义类型 不用了
    static CompOp interp_sv_comp_op(ast::ExprOps op) {
        static std::map<ast::ExprOps, CompOp> m = {
            {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
            {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
        };
        return m.at(op);
    }
    
    */
   
    //ast2defs_values
    // static Values interp_sv_value(const std::shared_ptr<ast::ConstValue> &cst_val) {
    //     Values val;//need ql
    //     if (auto int_ = std::dynamic_pointer_cast<ast::ConstInt>(cst_val)) {
    //         val.set_int(int_->value);
    //     } else if (auto float_ = std::dynamic_pointer_cast<ast::ConstFloat>(cst_val)) {
    //         val.set_float(float_->value);
    //     } else if (auto str_ = std::dynamic_pointer_cast<ast::ConstString>(cst_val)) {
    //         val.set_str(str_->value);
    //     } else {
    //         throw InternalError("Unexpected sv value type");
    //     }
    //     return val;
    // }


    struct inter_w{
        std::vector< std::string > vec1;
        std::vector< int > vec2;
        inter_w(std::vector< std::string > in_vec1,std::vector< int > in_vec2):
        vec1(in_vec1),vec2(in_vec2){}
    };

//for where
    static void interp_where(const std::shared_ptr<ast::Expression> &root, std::vector<std::string> &_used_table,std::map<std::string,std::string> &_used_tab2alt,
                             std::vector<std::string> &v1, std::vector<int> &v2){

        if(auto x = std::dynamic_pointer_cast<ast::ConstInt>(root)){
            v1.push_back(std::to_string(x->value));
            v2.push_back(EXPR_TYPE_INT);
        }
        else if(auto x = std::dynamic_pointer_cast<ast::ConstFloat>(root)){
            v1.push_back(std::to_string(x->value));
            v2.push_back(EXPR_TYPE_FLOAT);
        }
        else if(auto x = std::dynamic_pointer_cast<ast::ConstString>(root)){
            v1.push_back(x->value);
            v2.push_back(EXPR_TYPE_CHAR);
        }
        else if(auto x = std::dynamic_pointer_cast<ast::Column>(root)){ 
        std::string _table_name = x->tabName;
        std::string _alt_name;
        if(!_table_name.empty()){
            v1.push_back(_used_tab2alt[_table_name]+'.'+x->colName);
            v2.push_back(EXPR_TYPE_COLUMN);
        }
        else{
            for(int i=0;i<_used_table.size();i++){
                if(SmManager::db.get_table(_alt2tab[_used_table[i]]).is_col(x->colName)){
                    _alt_name = _used_table[i];
                    break;
                }
            }
            if(_alt_name.empty()){
                std::cout<<"no such where col"<<std::endl;
            }
            v1.push_back(_alt_name+'.'+x->colName);
            v2.push_back(EXPR_TYPE_COLUMN);
        }
        }
        else if (auto x = std::dynamic_pointer_cast<ast::BasicExpr>(root)){
            v1.push_back(std::to_string(x->op));
            v2.push_back(EXPR_TYPE_OPERATOR);
            interp_where(x->lExpr, _used_table,_used_tab2alt,v1,v2);
            interp_where(x->rExpr, _used_table,_used_tab2alt,v1,v2);
        }
        else{
            throw WindowsError();
        }
    

        return;

    }


};



#endif