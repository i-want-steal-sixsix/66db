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
    static std::map<std::string,std::vector<std::string>> tab2col;

    static int interp_sql(const std::shared_ptr<ast::ASTNode> &root) {
        bool exit_flag = false;
        if (auto x = std::dynamic_pointer_cast<ast::HelpStmt>(root)) {
            std::cout
                << "Supported SQL syntax:\n"
                   "\n"
                   "command:\n"
                   "  CREATE DATABASE db_name;\n"
                   "  SHOW DATABASE;\n"
                   "  DROP DATABASE db_name;\n"
                   "  CREATE TABLE table_name (Col1 type,Col2 type,...);\n"
                   "  SHOW TABLE;\n"
                   "  DROP TABLE table_name;\n"
                   "  INSERT INTO table_name [(Col1,Col2,...)] VALUES (value1,value2,...)\n"
                   "  DELETE FROM table_name [WHERE where_clause]\n"
                   "  UPDATE table_name SET column_name = value [, column_name = value ...] [WHERE where_clause]\n"
                   "  SELECT selector FROM table_name [WHERE where_clause]\n"
                   "type:\n"
                   "  {INT | FLOAT | CHAR(n)}\n"
                   "where_clause:\n"
                   "  any bool expression\n"
                   "column_name:\n"
                   "  [table_name. | alt_name.] column_name\n"
                   "op:\n"
                   "  { + | - | * | / | = | <> | < | > | <= | >= | AND | OR }\n"
                   "selector:\n"
                   "  {* | column_name [, column_name ...]}\n";

            //sm
        }
        else if (auto x = std::dynamic_pointer_cast<ast::ExitStmt>(root)) {
            if(SmManager::sys_state == SYS_DATABASE){
                SmManager::close_db();
            }
            else if(SmManager::sys_state == SYS_HOME){
                exit_flag = true;
            }
        }
        else if (auto x = std::dynamic_pointer_cast<ast::CreateDBStmt>(root)) {
            SmManager::create_db(x->dbName);
        }
        else if (auto x = std::dynamic_pointer_cast<ast::OpenDBStmt>(root)) {
            SmManager::open_db(x->dbName);
        } 
        else if (auto x = std::dynamic_pointer_cast<ast::DropDBStmt>(root)) {
            SmManager::drop_db(x->dbName);
        }
        else if (auto x = std::dynamic_pointer_cast<ast::ShowDBStmt>(root)) {
            SmManager::show_dbs();
        } 
        else if (auto x = std::dynamic_pointer_cast<ast::ShowTableStmt>(root)) {
            SmManager::show_tables();
        } 
        else if (auto x = std::dynamic_pointer_cast<ast::CreateTableStmt>(root)) {
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
            //tab2col
            tab2col[_tab_name] = _col_names;
        } 
        else if (auto x = std::dynamic_pointer_cast<ast::DropTableStmt>(root)) {
            SmManager::drop_table(x->name);

            //ql
        }
        else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(root)) {
            //std::cout<<"insert start"<<std::endl;
            std::vector<Values*> values;
            std::vector<std::shared_ptr<ast::ConstValue>> _values = x->values;
            for (int i=0;i<_values.size();i++){
                values.push_back(interp_ast_value(_values[i]));
                //std::cout<<"values:"<<interp_ast_value(_values[i])->type<<std::endl;
            }
            //std::cout<<"insert ???"<<std::endl;
            QlManager::insert_into(x->tableName, values);
        }
        else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(root)) {
            //conds
            std::vector< std::string > v1;      // 按顺序存放节点数据
            std::vector< int > v2;              // 按顺序存放节点类型
            interp_where(x->condition, v1, v2,x->table);
            inter_w conds(v1,v2);
            //std::cout<<"conds'ok"<<std::endl;
            QlManager::delete_from(x->table, conds);
      
        }
        else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(root)) {
            //conds
            std::vector< std::string > v1;      // 按顺序存放节点数据
            std::vector< int > v2;              // 按顺序存放节点类型
            interp_where(x->condition, v1, v2,x->table);
            inter_w conds(v1,v2);

            std::vector<std::string> set_cols;
            std::vector<inter_w *> set_exprs;

            for(int i=0;i<x->updList.size();i++){ 
                set_cols.push_back(x->table+"."+x->updList[i]->column->colName);
                std::vector< std::string > t_v1;
                std::vector< int > t_v2;
                interp_where(x->updList[i]->expression, t_v1, t_v2, x->table);
                inter_w *new_expr = new inter_w(t_v1,t_v2);

                set_exprs.push_back(new_expr);
            }

            QlManager::update_set(x->table, set_cols, set_exprs, conds);

        }
        else if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(root)) {
            _alt2tab.clear();
            //std::cout<<"sel"<<std::endl;
            //tab_names
            //std::cout<<"size of alt2tab"<<_alt2tab.size()<<std::endl;
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
                    std::cout<<"[Error] Repeated alt_name." << std::endl;
                    return 0;
                }
                //_used_table
                _used_table.push_back(_alt_name);
                //_used_tab2alt
                _used_tab2alt[_table_name] = _alt_name;
            }
            //std::cout<<"tab'ok"<<std::endl;
            //conds
            std::vector< std::string > v1;      // 按顺序存放节点数据
            std::vector< int > v2;              // 按顺序存放节点类型
            interp_where(x->condition, _used_table,_used_tab2alt, v1, v2);
            inter_w conds(v1,v2);
            //std::cout<<"conds'ok"<<std::endl;
            //sel_cols
            std::vector<SelColMeta> sel_cols;       
            std::vector<std::shared_ptr<ast::SelColumn>> _columns = x->columns;
            std::vector<std::shared_ptr<ast::SelColumn>>::iterator ic;

            std::string _table_name;
            std::string _col_name;
            std::string _alt_name;
            //std::cout<<"_columns.size:"<<_columns.size()<<std::endl;
            if(_columns.size() == 0){
                //std::cout<<"select *"<<std::endl;
                for(int i=0;i<tab_names.size();i++){
                    //std::cout<<"0"<<std::endl;
                    std::string tab_name = tab_names[i].tab_name;
                    //std::cout<<"1"<<std::endl;
                    std::vector<ColMeta> &_in_names = SmManager::db.tabs[tab_name].cols;
                    //std::cout<<"2"<<std::endl;
                    _table_name = tab_names[i].alt_name;
                    for(int j=0;j<_in_names.size();j++){
                        //std::cout<<"3"<<std::endl;
                        _col_name = _in_names[j].name;
                        _alt_name = "";
                        SelColMeta sel_col(_table_name,_col_name,_alt_name);
                        sel_cols.push_back(sel_col);
                    }
                    //std::cout<<"4"<<std::endl;
                }
            }
            else{
                for(ic=_columns.begin();ic<_columns.end();ic++){
                    _table_name = ic->get()->column->tabName;
                    _col_name = ic->get()->column->colName;
                    _alt_name = ic->get()->altName;
                    // 
                    if(_col_name.empty()){
                        std::cout<<"[Error] No column."<< std::endl;
                        return 0;
                    }

                    if(_table_name.empty()){
                        bool tab_flag = false;
                        for(int i=0;i<_used_table.size();i++){
                            if(SmManager::db.get_table(_alt2tab[_used_table[i]]).is_col(_col_name)){
                                if(tab_flag){
                                    std::cout << "[Error] Multiple possibility for column: " << _col_name << std::endl;
                                    return 0;
                                }
                                _table_name = _used_table[i];
                                tab_flag = true;
                            }
                        }
                    }
                    if(_table_name.empty()){
                        std::cout << "[Error] No column." << std::endl;
                        return 0;
                    }
                    
                    SelColMeta sel_col(_table_name,_col_name,_alt_name);
                    sel_cols.push_back(sel_col);
                }
            }

            /*
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
            */

            QlManager::select_from(sel_cols, tab_names,conds);
        }
        
        else {
            std::cout << "[Error] Unexpected AST root." << std::endl;
        }

        if(exit_flag){
            return 1;
        }
        return 0;

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
    static Values* interp_ast_value(const std::shared_ptr<ast::ConstValue> &cst_val) {
        //std::cout<<"interp_ast_value start"<<std::endl;
        uint8_t _type;
        uint16_t _length;
        bool _is_null;
        
        if (auto _int = std::dynamic_pointer_cast<ast::ConstInt>(cst_val)) {
            //std::cout<<"consrtuct int"<<std::endl;
            _is_null = false;
            _type = COL_TYPE_INT,
            _length = 4;
            Values* val = new Values(_type,_length,_is_null);
            val->int_val = _int->value;
            return val;
        } else if (auto _float = std::dynamic_pointer_cast<ast::ConstFloat>(cst_val)) {
            //std::cout<<"consrtuct float"<<std::endl;
            _is_null = false;
            _type = COL_TYPE_FLOAT,
            _length = 4;
            Values* val = new Values(_type,_length,_is_null);
            val->float_val = _float->value;
            return val;
        } else if (auto _str = std::dynamic_pointer_cast<ast::ConstString>(cst_val)) {
            //std::cout<<"consrtuct char"<<std::endl;
            _is_null = false;
            _type = COL_TYPE_CHAR;
            _length = _str->value.size();
            Values* val = new Values(_type,_length,_is_null);
            val->char_val = _str->value;
            return val;
        } else {
            //std::cout<<"consrtuct null"<<std::endl;
            _is_null = true;
            _type = 20;
            _length = 20;
            Values* val = new Values(_type,_length,_is_null);
            return val;
        }
        
        
    }


    

//for where in select
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
            v1.push_back(_table_name+'.'+x->colName);
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
                std::cout<<"[Error] No column."<<std::endl;
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

    //for where in delete update
    static void interp_where(const std::shared_ptr<ast::Expression> &root,std::vector<std::string> &v1, std::vector<int> &v2,std::string table_name){
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
                v1.push_back(_table_name+'.'+x->colName);
                v2.push_back(EXPR_TYPE_COLUMN);
            }
            else{
                //std::cout << "WHERE table: " << table_name << std::endl;
                if(SmManager::db.get_table(table_name).is_col(x->colName)){
                    v1.push_back(table_name+'.'+x->colName);
                    v2.push_back(EXPR_TYPE_COLUMN);
                }
                else{
                    std::cout<<"[ERROR] No column.";
                }
            }
        }
        else if (auto x = std::dynamic_pointer_cast<ast::BasicExpr>(root)){
            v1.push_back(std::to_string(x->op));
            v2.push_back(EXPR_TYPE_OPERATOR);
            interp_where(x->lExpr,v1,v2,table_name);
            interp_where(x->rExpr,v1,v2,table_name);
        }
        else{
            throw WindowsError();
        }
    

        return;
    }

};



#endif