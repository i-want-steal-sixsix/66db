#include "sm_manager.h"
#include "../pf/pf_fm.h"
#include "../pf/pf_pm.h"
#include "../defs.h"
#include "../managers.h"
#include <sys/stat.h>
#include <fstream>

DbMeta SmManager::db;
int SmManager::sys_state = SYS_HOME;

bool SmManager::is_dir(const std::string &db_name){
    return PfFileManager::is_file(DB_BASE_DIR + db_name + DB_DBF);
}

void SmManager::create_db(std::string &db_name){
    // 判断数据库名是不是过长或过短
    if(db_name.size() > DB_MAX_NAME_LENGTH || db_name.size() == 0){
        throw DatabaseNameLengthError(db_name);
    }
    // 判断数据库是否存在
    struct _stat dirStat;
    if(is_dir(db_name)){
        throw DatabaseExistsError(db_name);
    }
    // 新建dbf与dat文件
    PfFileManager::create_file(DB_BASE_DIR + db_name + DB_DBF);
    PfFileManager::create_file(DB_BASE_DIR + db_name + DB_DAT);

    //写dat
    int fd = PfFileManager::open_file(DB_BASE_DIR + db_name + DB_DAT);
    DatFileHdr newDatHdr(1);
    PfPageManager::blank_page(fd, 0);
    PfPageManager::write_page(fd, 0, (uint8_t *)&newDatHdr, sizeof(newDatHdr));
    PfPageManager::blank_page(fd, 1);
    PfFileManager::close_file(fd);

    // 写dbf
    fd = PfFileManager::open_file(DB_BASE_DIR + db_name + DB_DBF);
    // 写文件头
    DbfFileHdr newFileHdr(db_name, 0, 2);
    PfPageManager::blank_page(fd, 0);
    PfPageManager::write_page(fd, 0, (uint8_t *)&newFileHdr, sizeof(newFileHdr));
    // 写一级索引头
    DbfTabIdxPageHdr newPgIdxHdr;
    PfPageManager::blank_page(fd, 1);
    PfPageManager::write_page(fd, 1, (uint8_t *)&newPgIdxHdr, sizeof(newPgIdxHdr));
    // 写空页
    PfPageManager::blank_page(fd, 2);
    // 写dbmeta
    db.name = db_name;
    PfFileManager::close_file(fd);

    // 切换状态
    SmManager::sys_state = SYS_DATABASE;
    return;
}

/*
void SmManager::drop_db(const std::string &db_name){
    // 需要在系统首页模式下执行
    if(sys_state != SYS_HOME){
        throw DatabaseNotHomeModeError();
    }
    if(!is_dir(db_name)){
        throw DatabaseNotFoundError(db_name);
    }
    if(remove((DB_BASE_DIR + db_name + DB_DBF).c_str()) != 0){
        throw WindowsError();
    }
    if(remove((DB_BASE_DIR + db_name + DB_DAT).c_str()) != 0){
        throw WindowsError();
    }
    return;
}
*/

/* 还未更新manager内部db的情况！！！！！！ */
void SmManager::create_table(std::string &tab_name, std::vector<ColDef> &col_defs){
    // 未指定数据库
    if(sys_state != SYS_DATABASE){
        throw DatabaseNotSelectedError();
    }
    // 是否过长
    if(tab_name.size() > TAB_MAX_NAME_LENGTH || tab_name.size() == 0){
        throw TableNameLengthError(tab_name);
    }
    // 检查表是否已经存在
    if(db.is_table(tab_name)){
        throw TableExistsError(tab_name);
    }
    // 开启文件
    int dbf_fd = PfFileManager::open_file(DB_BASE_DIR + db.name + DB_DBF);
    int dat_fd = PfFileManager::open_file(DB_BASE_DIR + db.name + DB_DAT);
    
    Page *front_page = sys_page_mgr.fetch_page(dbf_fd, 0);

    uint16_t new_freeID = first_free_page(front_page->buf);
    
    // 创建dataidx
    uint16_t newDatIdxID = create_new_page(dbf_fd, front_page);
    Page *datidx_page = sys_page_mgr.fetch_page(dbf_fd, newDatIdxID);
    DbfDatIdxPageHdr newDatIdxHdr;
    newDatIdxHdr.record_count = 0;
    PfPageManager::blank_page(dbf_fd, newDatIdxID);
    PfPageManager::write_page(dbf_fd, newDatIdxID, (uint8_t *)&newDatIdxHdr, sizeof(newDatIdxHdr));
    // dat文件新建一页
    Page *front_page_dat = sys_page_mgr.fetch_page(dat_fd, 0);
    uint16_t new_freeID_dat = create_new_page(dat_fd, front_page_dat);
    // dbf dataidx新增记录
    DbfDatIdxRec newDatIdxRec(new_freeID_dat);
    add_datidx_rec(datidx_page, &newDatIdxRec);
    // dat文件更新
    Page *dat_page = sys_page_mgr.fetch_page(dat_fd, new_freeID_dat);
    sys_page_mgr.flush_page(dat_page);

    // 创建struct
    uint16_t newStructID = create_new_page(dbf_fd, front_page);
    DbfStructPageHdr newStructHdr;
    PfPageManager::write_page(dbf_fd, newStructID, (uint8_t *)&newStructHdr, sizeof(newStructHdr));
    // 写记录
    uint16_t nowStructID = newStructID;
    Page *struct_page = sys_page_mgr.fetch_page(dbf_fd, newStructID);
    for(int i = 0; i < col_defs.size(); i++){
        if(is_struct_full(struct_page->buf)){
            // 添加新的一页
            set_next_page(struct_page->buf, nowStructID);
            sys_page_mgr.flush_page(struct_page);
            nowStructID = create_new_page(dbf_fd, front_page);
            PfPageManager::write_page(dbf_fd, nowStructID, (uint8_t *)&newStructHdr, sizeof(newStructHdr));
            struct_page = sys_page_mgr.fetch_page(dbf_fd, nowStructID);
        }
        DbfStructRec newRec(col_defs[i].name, col_defs[i].type, col_defs[i].len, col_defs[i].option);
        add_struct_rec(struct_page, &newRec);
    }
    sys_page_mgr.flush_page(struct_page);

    std::cout << "struct OK" << std::endl;

    // 写tabidx
    uint16_t nowTabIdxID = 1;
    Page *tabidx_page = sys_page_mgr.fetch_page(dbf_fd, nowTabIdxID);
    // 寻找有位置的第一页
    while(is_tabidx_full(tabidx_page->buf)){
        if(next_page(tabidx_page->buf) == 0){
            nowTabIdxID = create_new_page(dbf_fd, front_page);
            DbfTabIdxPageHdr newPgIdxHdr;
            PfPageManager::write_page(dbf_fd, nowTabIdxID, (uint8_t *)&newPgIdxHdr, sizeof(newPgIdxHdr));
            tabidx_page = sys_page_mgr.fetch_page(dbf_fd, nowTabIdxID);
            break;
        }
        nowTabIdxID = next_page(tabidx_page->buf);
        tabidx_page = sys_page_mgr.fetch_page(dbf_fd, nowTabIdxID);
    }
    DbfTabIdxRec newTabIdxRec(tab_name, newStructID, newDatIdxID);
    add_tabidx_rec(tabidx_page, &newTabIdxRec);
    sys_page_mgr.flush_page(tabidx_page);

    sys_page_mgr.flush_all();               // 必须要先 flush_all

    PfFileManager::close_file(dbf_fd);
    PfFileManager::close_file(dat_fd);

    return;
}