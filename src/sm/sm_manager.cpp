#include "sm_manager.h"
#include "../pf/pf_fm.h"
#include "../pf/pf_pm.h"
#include "../rm/rm_manager.h"
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

    PfFileManager::close_file(fd);

    // 不切换状态
    SmManager::sys_state = SYS_HOME;
    return;
}

void SmManager::drop_db(const std::string &db_name){
    // 检查是不是首页模式
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

void SmManager::open_db(const std::string &db_name){
    // 检查是不是首页模式
    if(sys_state != SYS_HOME){
        throw DatabaseNotHomeModeError();
    }
    if(!is_dir(db_name)){
        throw DatabaseNotFoundError(db_name);
    }
    db.name = db_name;
    int fd = PfFileManager::open_file(DB_BASE_DIR + db_name + DB_DBF);
    Page *front_page = sys_page_mgr.fetch_page(fd, 0);
    uint16_t tab_count = front_page->buf[32] + ((uint16_t)(front_page->buf[33])<<8);
    Page *tabidx_page = sys_page_mgr.fetch_page(fd, 1);
    int now_page = 1;
    // 查询所有表
    while(tab_count > 0){
        // 取记录
        for(int i = 0; i < DBF_TABIDX_BMP_SIZE; i++){
            if(!Bitmap::test(tabidx_page->buf + DBF_HEADER_SIZE, i)){
                continue;
            }
            TabMeta tab_meta;
            tab_meta.rid = Rid(now_page, i);
            DbfTabIdxRec tabIdxRec = get_tabidx_rec(tabidx_page, i);

            // 表名
            std::string tmp_tab_name = get_name(tabIdxRec.tabname, TAB_MAX_NAME_LENGTH);
            tab_meta.name = tmp_tab_name;

            // 加载 表结构信息
            uint16_t struct_pageID = tabIdxRec.structidx;
            while(struct_pageID != 0){
                Page *struct_page = sys_page_mgr.fetch_page(fd, struct_pageID);
                for(int j = 0; j < DBF_STRUCT_BMP_SIZE; j++){
                    if(!Bitmap::test(struct_page->buf + DBF_HEADER_SIZE, j)){
                        continue;
                    }
                    ColMeta col_meta;

                    DbfStructRec structRec = get_struct_rec(struct_page, j);
                    std::string tmp_col_name = get_name(structRec.colname, COL_MAX_NAME_LENGTH);
                    col_meta.name = tmp_col_name;

                    col_meta.type = structRec.datatype;
                    col_meta.len = structRec.length;
                    col_meta.option = structRec.option;
                    
                    tab_meta.cols.push_back(col_meta);
                }
                struct_pageID = next_page(struct_page->buf);
                sys_page_mgr.flush_page(struct_page);
            }

            // 加载 表数据页索引信息
            uint16_t datidx_pageID = tabIdxRec.dataidx;
            while(datidx_pageID != 0){
                Page *datidx_page = sys_page_mgr.fetch_page(fd, datidx_pageID);
                for(int j = 0; j < DBF_DATIDX_BMP_SIZE; j++){
                    if(!Bitmap::test(datidx_page->buf + DBF_HEADER_SIZE, j)){
                        continue;
                    }
                    DbfDatIdxRec datIdxRec = get_datidx_rec(datidx_page, j);
                    tab_meta.pages.push_back(datIdxRec.page_no);
                }
                datidx_pageID = next_page(datidx_page->buf);
                sys_page_mgr.flush_page(datidx_page);
            }

            db.tabs[tab_meta.name] = tab_meta;
            tab_count--;
        }
        // 获取下一页

        if(tab_count > 0){
            now_page = next_page(tabidx_page->buf);
            tabidx_page = sys_page_mgr.fetch_page(fd, now_page);
        }
    }

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    sys_state = SYS_DATABASE;

    return;
}

void SmManager::close_db(){
    // 由于每次ddl语句会直接修改文件，所以只需要把内存中的释放再修改状态即可
    db.tabs.clear();
    sys_state = SYS_HOME;
    return;
}

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

    uint16_t record_len = 1 + (col_defs.size() + 7)/8;
    // 计算行长度
    for(int i = 0; i < col_defs.size(); i++){
        record_len = record_len + col_defs[i].len;
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
    PfPageManager::set_page(datidx_page, (uint8_t *)&newDatIdxHdr, 0, DBF_HEADER_SIZE-1);
    // dat文件新建一页
    Page *front_page_dat = sys_page_mgr.fetch_page(dat_fd, 0);
    uint16_t new_freeID_dat = create_new_page(dat_fd, front_page_dat);
    sys_page_mgr.flush_page(front_page_dat);

    RmManager::init_fixlen_page(dat_fd, new_freeID_dat, record_len);  // 目前只支持定长记录

    // dbf dataidx新增记录
    DbfDatIdxRec newDatIdxRec(new_freeID_dat);
    add_datidx_rec(datidx_page, &newDatIdxRec);

    // 创建struct
    uint16_t newStructID = create_new_page(dbf_fd, front_page);
    DbfStructPageHdr newStructHdr;
    // 写记录
    uint16_t nowStructID = newStructID;
    Page *struct_page = sys_page_mgr.fetch_page(dbf_fd, newStructID);
    PfPageManager::set_page(struct_page, (uint8_t *)&newStructHdr, 0, DBF_HEADER_SIZE-1);
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
    uint16_t newslot = add_tabidx_rec(tabidx_page, &newTabIdxRec);
    sys_page_mgr.flush_page(tabidx_page);

    table_count_inc(front_page);
    sys_page_mgr.flush_file(dbf_fd);               // 必须要先 flush_all
    sys_page_mgr.flush_file(dat_fd);
    PfFileManager::close_file(dbf_fd);
    PfFileManager::close_file(dat_fd);

    // 写入manager内部
    TabMeta newTab;
    newTab.name = tab_name;
    for(int i = 0; i < col_defs.size(); i++){
        newTab.cols.push_back(ColMeta(col_defs[i].name, col_defs[i].type, col_defs[i].len, col_defs[i].option));
    }
    newTab.rid = Rid(nowTabIdxID, newslot);
    newTab.pages.push_back(new_freeID_dat);
    db.tabs[tab_name] = newTab;

    return;
}

void SmManager::drop_table(const std::string &tab_name){
    // 未指定数据库
    if(sys_state != SYS_DATABASE){
        throw DatabaseNotSelectedError();
    }
    // 检查表是否存在
    if(!db.is_table(tab_name)){
        throw TableNotFoundError(tab_name);
    }
    // 开启文件
    int dbf_fd = PfFileManager::open_file(DB_BASE_DIR + db.name + DB_DBF);
    int dat_fd = PfFileManager::open_file(DB_BASE_DIR + db.name + DB_DAT);

    // 清除表的记录
    Page *dat_front_page = sys_page_mgr.fetch_page(dat_fd, 0);
    uint16_t free_dat_page = first_free_page(dat_front_page->buf);
    for(int i = 0; i < db.tabs[tab_name].pages.size(); i++){
        Page *tmp_page = sys_page_mgr.fetch_page(dat_fd, db.tabs[tab_name].pages[i]);
        set_next_page(tmp_page->buf, free_dat_page);
        free_dat_page = db.tabs[tab_name].pages[i];
        tmp_page->mark_dirty();
        sys_page_mgr.flush_page(tmp_page);
    }
    set_free_page(dat_front_page->buf, free_dat_page); 
    dat_front_page->mark_dirty();
    sys_page_mgr.flush_page(dat_front_page);
    sys_page_mgr.flush_file(dat_fd);
    PfFileManager::close_file(dat_fd);

    // 清除dbf的相关记录
    Page *dbf_front_page = sys_page_mgr.fetch_page(dbf_fd, 0);
    uint16_t free_page = first_free_page(dbf_front_page->buf);

    Page *dbf_tabidx_page = sys_page_mgr.fetch_page(dbf_fd, db.tabs[tab_name].rid.page_no);
    DbfTabIdxRec tmprec =  get_tabidx_rec(dbf_tabidx_page, db.tabs[tab_name].rid.slot_no);
    Bitmap::reset(dbf_tabidx_page->buf+DBF_HEADER_SIZE, db.tabs[tab_name].rid.slot_no);
    // 记录数-1
    dbf_tabidx_page->buf[3]--;
    dbf_tabidx_page->mark_dirty();
    sys_page_mgr.flush_page(dbf_tabidx_page);

    // 清除struct
    uint16_t now_pageid = tmprec.structidx;
    while(now_pageid != 0){
        Page *now_page = sys_page_mgr.fetch_page(dbf_fd, now_pageid);
        int tmp_pageid = next_page(now_page->buf); 
        set_next_page(now_page->buf, free_page);
        Bitmap::init(now_page->buf+DBF_HEADER_SIZE,DBF_STRUCT_BMP_SIZE/8);
        now_page->mark_dirty();
        sys_page_mgr.flush_page(now_page);
        free_page = now_pageid;
        now_pageid = tmp_pageid;
    }
    set_free_page(dbf_front_page->buf, free_page);
    // 清除datidx
    now_pageid = tmprec.dataidx;
    while(now_pageid != 0){
        Page *now_page = sys_page_mgr.fetch_page(dbf_fd, now_pageid);
        int tmp_pageid = next_page(now_page->buf); 
        set_next_page(now_page->buf, free_page);
        Bitmap::init(now_page->buf+DBF_HEADER_SIZE,DBF_DATIDX_BMP_SIZE/8);
        now_page->mark_dirty();
        sys_page_mgr.flush_page(now_page);
        free_page = now_pageid;
        now_pageid = tmp_pageid;
    }
    set_free_page(dbf_front_page->buf, free_page);
    table_count_dec(dbf_front_page);

    sys_page_mgr.flush_file(dbf_fd);
    PfFileManager::close_file(dbf_fd);

    // 清除manager内记录
    db.tabs.erase(tab_name);

    return;
}

/* 这部分需要结合printer，目前只是标准输入输出 */
void SmManager::show_tables(){
    std::map<std::string, TabMeta>::iterator iter = db.tabs.begin();
    std::cout << db;
    return;
}