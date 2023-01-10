#ifndef SM_MANAGER_H
#define SM_MANAGER_H

#pragma once
#include "sm_meta.h"
#include "../managers.h"
#include "../rm/rm_manager.h"
#include <iostream>
#include <string>

enum SysState {
    SYS_HOME, SYS_DATABASE
};

struct ColDef {
    std::string name;
    uint8_t type;
    uint16_t len;
    uint8_t option;

    ColDef() = default;
    ColDef(std::string name_, uint8_t type_, uint16_t len_, uint8_t option_)
    : name(std::move(name_)), type(type_), len(len_), option(option_) {}

};

class SmManager {
public:
    static int sys_state;
    static DbMeta db;
    //static std::map<std::string, std::unique_ptr<RmFileHandle>> fhs;
    //static std::map<std::string, std::unique_ptr<IxIndexHandle>> ihs;

    // 数据库管理
    static bool is_dir(const std::string &db_name);

    static void create_db(std::string &db_name);

    static void drop_db(const std::string &db_name);

    static void open_db(const std::string &db_name);

    static void close_db();

    // 表管理
    static void show_tables();

    static void create_table(std::string &tab_name, std::vector<ColDef> &col_defs);

    static void drop_table(const std::string &tab_name);

    // 记录文件管理 -- ql 调用
    static uint16_t add_data_page(int dat_fd, std::string tab_name, uint16_t rec_size){
        // 新建新的 dat 的页
        Page *dat_front_page = sys_page_mgr.fetch_page(dat_fd, 0);
        uint16_t dat_new_page = create_new_page(dat_fd, dat_front_page);
        RmManager::init_fixlen_page(dat_fd, dat_new_page, rec_size);
        
        // 打开 dbf 文件
        int dbf_fd = PfFileManager::open_file(DB_BASE_DIR+db.name+DB_DBF);
        // 找到有空位置的datidx页
        Rid tmprid = db.tabs[tab_name].rid;

        Page *dbf_front_page = sys_page_mgr.fetch_page(dbf_fd, 0);
        Page *tabidx_page = sys_page_mgr.fetch_page(dbf_fd, tmprid.page_no);

        DbfTabIdxRec tmptabidx = get_tabidx_rec(tabidx_page, tmprid.slot_no);
        uint16_t now_datidx_pageid = tmptabidx.dataidx;
        Page *now_datidx_page = sys_page_mgr.fetch_page(dbf_fd, now_datidx_pageid);
        while(is_datidx_full(now_datidx_page->buf)){
            uint16_t tmpnext = next_page(now_datidx_page->buf);
            // 创建新的datidx页
            if(tmpnext == 0){
                uint16_t tmpnext = create_new_page(dbf_fd, dbf_front_page);
                DbfDatIdxPageHdr tmphdr;
                PfPageManager::write_page(dbf_fd, tmpnext, (uint8_t*)&tmphdr, DBF_HEADER_SIZE);
            }
            sys_page_mgr.flush_page(now_datidx_page);
            now_datidx_page = sys_page_mgr.fetch_page(dbf_fd, tmpnext);
        }
        // 添加记录
        DbfDatIdxRec newRec(dat_new_page);
        add_datidx_rec(now_datidx_page, &newRec);

        PfFileManager::close_file(dbf_fd);

        // 往manager中添加数据
        db.tabs[tab_name].pages.push_back(dat_new_page);

        return dat_new_page;
    }


private:

    static std::string get_name(char *name, int max_len){
        std::string name_str;
        for(int i = 0; i < max_len; i++){
            if(name[i] == '\0') break;
            name_str.push_back(name[i]);
        }
        std::cout << name_str << " length: " << name_str.size() << std::endl;
        return name_str;
    }

    static bool is_tabidx_full(uint8_t *page){
        return page[2] == DBF_TABIDX_BMP_SIZE;
    };
    static bool is_struct_full(uint8_t *page){
        return page[2] == DBF_STRUCT_BMP_SIZE;
    }
    static bool is_datidx_full(uint8_t *page){
        uint16_t num = (uint16_t(page[3]) << 8) + page[2];
        return num == DBF_DATIDX_BMP_SIZE;
    }
    static int get_free_tabidx(uint8_t *page){
        return Bitmap::next_bit(0, page+DBF_HEADER_SIZE, DBF_TABIDX_BMP_SIZE, -1);
    }
    static int get_free_struct(uint8_t *page){
        return Bitmap::next_bit(0, page+DBF_HEADER_SIZE, DBF_STRUCT_BMP_SIZE, -1);
    }
    static int get_free_datidx(uint8_t *page){
        return Bitmap::next_bit(0, page+DBF_HEADER_SIZE, DBF_DATIDX_BMP_SIZE, -1);
    }
    static uint16_t next_page(uint8_t *page){
        return (uint16_t(page[2])<<8)+page[1];
    }
    static uint16_t first_free_page(uint8_t *front_page){
        return (uint16_t(front_page[35])<<8)+front_page[34];
    }

    static uint16_t create_new_page(int fd, Page *front_page){
        uint16_t new_page = first_free_page(front_page->buf);
        Page *page = sys_page_mgr.fetch_page(fd, new_page);
        if(next_page(page->buf) == 0){
            // 需要创建新的空白页
            if(new_page == DB_MAX_PAGE_COUNT){
                throw FileFullError(db.name);     //这里错误信息有待完善，不能指出是dbf还是dat
            }
            set_free_page(front_page->buf, new_page+1);
            PfPageManager::blank_page(fd, new_page+1);
        }
        else{
            set_free_page(front_page->buf, next_page(page->buf));
        }
        PfPageManager::blank_page(fd, new_page);
        sys_page_mgr.flush_page(page);
        front_page->mark_dirty();
        std::cout << "new page id: " << new_page << std::endl;
        return new_page;
    }

    static void set_free_page(uint8_t *buf, uint16_t freeID){
        buf[35] = uint8_t(freeID>>8);
        buf[34] = uint8_t(freeID&0xFF);
    }

    static void set_next_page(uint8_t *buf, uint16_t pageID){
        buf[2] = uint8_t(pageID>>8);
        buf[1] = uint8_t(pageID&0xFF);
    }
    
    static void table_count_inc(Page *page){
        page->buf[32]++;
        if(page->buf[32] == 0){
            page->buf[33]++;
            if(page->buf[33] == 0){
                throw DatabaseFullError(db.name);
            }
        }
        page->mark_dirty();
    }

    static void table_count_dec(Page *page){
        if(page->buf[32] == 0){
            page->buf[32] = 0xFF;
            page->buf[33]--;
        }
        else{
            page->buf[32]--;
        }
        page->mark_dirty();
    }

    static void add_datidx_rec(Page *page, DbfDatIdxRec *new_rec){
        if(is_datidx_full(page->buf)){
            throw PageFullError("Data Index");
        }
        int new_recID = get_free_datidx(page->buf);
        Bitmap::set(page->buf+DBF_HEADER_SIZE, new_recID);
        PfPageManager::set_page(page, (uint8_t*)new_rec,
        DBF_HEADER_SIZE+DBF_DATIDX_BMP_SIZE/8+new_recID*DBF_DATIDX_REC_SIZE,
        DBF_HEADER_SIZE+DBF_DATIDX_BMP_SIZE/8+new_recID*DBF_DATIDX_REC_SIZE+DBF_DATIDX_REC_SIZE-1);

        page->buf[3]++;
        if(page->buf[3] == 0){
            page->buf[4]++;
        }

        return;
    }

    static void add_struct_rec(Page *page, DbfStructRec *new_rec){
        if(is_struct_full(page->buf)){
            throw PageFullError("Structure");
        }
        int new_recID = get_free_struct(page->buf);
        Bitmap::set(page->buf+DBF_HEADER_SIZE, new_recID);
        PfPageManager::set_page(page, (uint8_t*)new_rec,
        DBF_HEADER_SIZE+DBF_STRUCT_BMP_SIZE/8+new_recID*DBF_STRUCT_REC_SIZE,
        DBF_HEADER_SIZE+DBF_STRUCT_BMP_SIZE/8+new_recID*DBF_STRUCT_REC_SIZE+DBF_STRUCT_REC_SIZE-1);

        page->buf[3]++;

        return;
    }

    static void add_tabidx_rec(Page *page, DbfTabIdxRec *new_rec){
        if(is_tabidx_full(page->buf)){
            throw PageFullError("Table Index");
        }
        int new_recID = get_free_tabidx(page->buf);

        Bitmap::set(page->buf+DBF_HEADER_SIZE, new_recID);

        PfPageManager::set_page(page, (uint8_t*)new_rec,
        DBF_HEADER_SIZE+DBF_TABIDX_BMP_SIZE/8+new_recID*DBF_TABIDX_REC_SIZE,
        DBF_HEADER_SIZE+DBF_TABIDX_BMP_SIZE/8+new_recID*DBF_TABIDX_REC_SIZE+DBF_TABIDX_REC_SIZE-1);

        page->buf[3]++;

        return;
    }

    static DbfTabIdxRec get_tabidx_rec(Page *page, int recID){
        DbfTabIdxRec record;
        uint8_t *ptr = (uint8_t*)&record;
        for(int i = 0; i < DBF_TABIDX_REC_SIZE; i++){
            ptr[i] = page->buf[i+DBF_HEADER_SIZE+DBF_TABIDX_BMP_SIZE/8+recID*DBF_TABIDX_REC_SIZE];
        }
        return record;
    }

    static DbfStructRec get_struct_rec(Page *page, int recID){
        DbfStructRec record;
        uint8_t *ptr = (uint8_t*)&record;
        for(int i = 0; i < DBF_STRUCT_REC_SIZE; i++){
            ptr[i] = page->buf[i+DBF_HEADER_SIZE+DBF_STRUCT_BMP_SIZE/8+recID*DBF_STRUCT_REC_SIZE];
        }
        return record;
    }

    static DbfDatIdxRec get_datidx_rec(Page *page, int recID){
        DbfDatIdxRec record;
        uint8_t *ptr = (uint8_t*)&record;
        for(int i = 0; i < DBF_DATIDX_REC_SIZE; i++){
            ptr[i] = page->buf[i+DBF_HEADER_SIZE+DBF_DATIDX_BMP_SIZE/8+recID*DBF_DATIDX_REC_SIZE];
        }
        return record;
    }

};

#endif