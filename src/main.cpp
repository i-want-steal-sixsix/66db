#include "error.h"
#include "sm/sm_def.h"
#include "sm/sm_manager.h"
#include "rm/rm_page_handle.h"
#include "rm/rm_scan.h"
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
    std::cout << "we got" << std::endl;
    SmManager::create_table(tabname2,testcol2);

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
    std::cout << "vreate ok" << std::endl;

    int test_fd = PfFileManager::open_file(DB_BASE_DIR+dbname+DB_DAT);

    std::cout << "open file" << std::endl;

    Page *test_page = sys_page_mgr.fetch_page(test_fd, 1);
    
    std::cout << "get page" << std::endl;

    // 记录构造：
    // 1字节记录头（保留）+ n位空标记（取整到 8位）+ sum 列长

    // 1 + (4+(+4)/8) + 4+40+4+4 = 54 字节

    // bitmap 大小 L：

    // (L * 54) + L / 8 <= 4000

    RmFixLenPageHandle newHandle(test_page, 72, 54);

    uint8_t test[54];
    for(int i = 0; i < 54; i++)
        test[i] = i;

    newHandle.insert_record(test);

    std::unique_ptr<FixLenRecord> test_rec;
    test_rec = newHandle.get_record(0);
    
    

    for(int i = 0; i < 54; i++)
        std::cout << int(test_rec.get()->data[i]) << " ";
    std::cout << std::endl;

    FixLenScan newScan(&newHandle);

    test[0] = 0xFC;
    newHandle.insert_record(test);

    test[0] = 0xFF;
    newHandle.insert_record(test);

    newScan.next();
    newScan.next();

    test_rec = newHandle.get_record(newScan.now_id);
    for(int i = 0; i < 54; i++)
        std::cout << int(test_rec.get()->data[i]) << " ";
    std::cout << std::endl;

    newHandle.delete_record(1);

    test[0] = 0xAA;
    newHandle.insert_record(test);

    test_rec = newHandle.get_record(newScan.now_id);
    for(int i = 0; i < 54; i++)
        std::cout << int(test_rec.get()->data[i]) << " ";
    std::cout << std::endl;

    return 0;
}