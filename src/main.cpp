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

    std::cout << "WTF!!!!" << std::endl;

    return 0;
}