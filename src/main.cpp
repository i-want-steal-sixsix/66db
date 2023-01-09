#include "error.h"
#include "sm/sm_def.h"
#include "sm/sm_manager.h"
#include "rm/rm_page_handle.h"
#include "rm/rm_scan.h"
#include "sqlparser/ast.h"
#include "sqlparser/parsor_bison.tab.h"
#include "sqlparser/lex.yy.h"
#include "interpret.h"
#include "defs.h"
#include "managers.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>

#include <vector>

PfPageManager sys_page_mgr;

int main(){

    // 生成data文件夹
    struct _stat dirStat;
    if(!((_stat(".\\data", &dirStat) == 0) && (dirStat.st_mode & _S_IFDIR))){
        std::cout << "Data missing" << std::endl;
        if(_mkdir(".\\data") != 0){
            throw(WindowsError());
        }
    }

    std::string dbname = "testdb";
    SmManager::create_db(dbname);

    
    // 测试
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

    SmManager::create_table(tabname,testcol);

    SmManager::create_table(tabname2,testcol2);

    SmManager::drop_table(tabname);

    SmManager::show_tables();

    SmManager::close_db();

    SmManager::open_db(dbname);
    

    while (1) {
        std::cout << "input> ";
        char *line_read = new char[210];
        std::cin.getline(line_read, 210, ';');

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
                std::cout << std::endl;
                Interp::interp_sql(ast::parse_tree);
            }
            yy_delete_buffer(buf);
        }
    }


    return 0;
}