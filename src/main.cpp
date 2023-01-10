#include "error.h"
#include "sm/sm_def.h"
#include "sm/sm_manager.h"
#include "rm/rm_page_handle.h"
#include "ql/ql_manager.h"
#include "ql/ql_scan.h"
#include "defs.h"
#include "managers.h"
#include "interpret.h"

#include "sqlparser/ast.h"
#include "sqlparser/parsor_bison.tab.h"
#include "sqlparser/lex.yy.h"


#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>

#include <vector>
#include <string.h>

PfPageManager sys_page_mgr;
std::map<std::string,std::string> Interp::_alt2tab;


int main(){

    // 生成data文件夹
    struct _stat dirStat;
    if(!((_stat(".\\data", &dirStat) == 0) && (dirStat.st_mode & _S_IFDIR))){
        std::cout << "Data missing" << std::endl;
        if(_mkdir(".\\data") != 0){
            throw(WindowsError());
        }
    }

    // 测试
    std::string dbname = "testdb1";
    std::string tabname = "testtab1";
    

    std::vector<ColDef> testcol{ColDef(std::string("col1"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col2"),COL_TYPE_CHAR,40,0),
                        ColDef(std::string("col3"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col4"),COL_TYPE_FLOAT,4,FIELD_OPT_NOTNULL|FIELD_OPT_PRIMKEY)};


    std::string tabname2 = "testtab2";

    std::vector<ColDef> testcol2{ColDef(std::string("col21"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col22"),COL_TYPE_CHAR,40,0),
                        ColDef(std::string("col23"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col24"),COL_TYPE_FLOAT,4,FIELD_OPT_NOTNULL|FIELD_OPT_PRIMKEY)};

    SmManager::create_db(dbname);
    SmManager::create_table(tabname,testcol);
    SmManager::create_table(tabname2,testcol2);
    std::cout<<"plen"<<SmManager::db.tabs[tabname].pages.size()<<std::endl;

/*
    SmManager::show_tables();

    SmManager::close_db();

    SmManager::open_db(dbname);
    std::cout << "open OK!" << std::endl;

    SmManager::show_tables();
    std::cout << "show OK!" << std::endl;

    SmManager::drop_table(tabname);
    std::cout << "drop OK!" << std::endl;

    SmManager::show_tables();
    std::cout << "show OK!" << std::endl;

    SmManager::close_db();

    SmManager::open_db(dbname);
    std::cout << "open OK!" << std::endl;

    SmManager::show_tables();
    std::cout << "show OK!" << std::endl;

    SmManager::create_table(tabname,testcol);

    SmManager::close_db();

    SmManager::open_db(dbname);
    std::cout << "open OK!" << std::endl;

    SmManager::show_tables();
    std::cout << "show OK!" << std::endl;
*/
    std::cout << "create ok" << std::endl;

    // 测试
    
    // 插入
    std::vector<Values> values{
    Values(COL_TYPE_INT, 4, false),
    Values(COL_TYPE_CHAR, 40, false),
    Values(COL_TYPE_INT, 4, false),
    Values(COL_TYPE_FLOAT, 4, false),
    };

    values[0].int_val = 123;
    values[1].char_val = "1st record!!!";
    values[2].int_val = -133;
    values[3].float_val = 1.543;

    QlManager::insert_into(tabname, values);
    values[2].is_null = true;
    QlManager::insert_into(tabname, values);
    values[2].is_null = false;
    values[1].char_val = "3rd record???";
    QlManager::insert_into(tabname, values);
    
    values[0].int_val = 456;
    values[1].char_val = "record 1";
    values[2].int_val = -166;
    values[3].float_val = -122.5;

    QlManager::insert_into(tabname2, values);
    values[1].is_null = true;
    QlManager::insert_into(tabname2, values);
    values[1].is_null = false;
    values[1].char_val = "record 3";
    values[3].float_val = 3232.3;
    QlManager::insert_into(tabname2, values);

    SmManager::close_db();
    std::cout<<"1_plen"<<SmManager::db.tabs[tabname].pages.size()<<std::endl;
    SmManager::open_db(dbname);
    std::cout<<"2_plen"<<SmManager::db.tabs[tabname].pages.size()<<std::endl;

    while (1) {
        std::cout << "input> ";
        char *line_read = new char[110];
        std::cin.getline(line_read, 100, ';');

        if (line_read == nullptr) {
            // EOF encountered
            break;
        }
        std::string line = line_read;
        line.append(";");
        delete line_read;
        //std::cout << "OK!INPUT IS: " << line << std::endl;

        if (!line.empty()) {
            YY_BUFFER_STATE buf = yy_scan_string(line.c_str());
            if (yyparse() == 0) {
                if (ast::parse_tree != nullptr) {
                    ast::parse_tree.get()->debug_print();
                }
                Interp::interp_sql(ast::parse_tree);
            }
            yy_delete_buffer(buf);
        }
    }

    std::cout << "test ends!" << std::endl;

    return 0;
}