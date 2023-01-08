#include "error.h"
#include "sm/sm_manager.h"
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

    // 测试
    std::string dbname = "testdb1";
    std::string tabname = "testtab1";
    SmManager::create_db(dbname);
/*
struct ColDef {
    std::string name;
    uint8_t type;
    uint16_t len;
    uint8_t option;
*/
    std::vector<ColDef> testcol{ColDef(std::string("col1"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col2"),COL_TYPE_CHAR,40,0),
                        ColDef(std::string("col3"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col4"),COL_TYPE_FLOAT,4,FIELD_OPT_NOTNULL|FIELD_OPT_PRIMKEY)};

    SmManager::create_table(tabname,testcol);

    std::string tabname2 = "testtab2";

    std::vector<ColDef> testcol2{ColDef(std::string("col21"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col22"),COL_TYPE_CHAR,40,0),
                        ColDef(std::string("col23"),COL_TYPE_INT,4,FIELD_OPT_NOTNULL),
                        ColDef(std::string("col24"),COL_TYPE_FLOAT,4,FIELD_OPT_NOTNULL|FIELD_OPT_PRIMKEY)};

    SmManager::create_table(tabname2,testcol2);

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

    return 0;
}