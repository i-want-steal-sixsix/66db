#include "ql_scan.h"

QlFixLenScan::QlFixLenScan(int fd_, std::string tab_name_){
    // 初始化
    std::cout << "OK!" << std::endl;
    fd = fd_;
    now_pageid = 0;
    now_id = -1;
    tab_name = tab_name_;

    col_size = SmManager::db.tabs[tab_name].cols.size();

    // 初始化原始记录
    uint16_t len_sum = 0;
    rec_header = 0;
    for(int i = 0; i < col_size; i++){
        uint16_t tmplen = SmManager::db.tabs[tab_name].cols[i].len;
        RecordRaw *tmpRaw = new RecordRaw(tmplen);
        rec_raws.push_back(tmpRaw);
        len_sum += tmplen;
    }
    std::cout << "OK1!" << std::endl;
    // 计算记录长度 与 bitmap长度
    rec_size = 1 + (col_size+7)/8 + len_sum;
    for(bmp_size = 0; bmp_size/8+bmp_size*rec_size < PAGE_SIZE-DAT_HEADER_SIZE; bmp_size+=8);
    bmp_size-=8;

    /* 注意，为了节省时间，这里没有检查bmp+size是否为0 */

    // 获取初始页
    // std::cout << "OK1.5!" << std::endl;
    std::cout<<"now_page_id"<<now_pageid<<"plen"<<tab_name<<std::endl;
    // std::cout << "sm:" << SmManager::db.tabs[tab_name].pages[now_pageid]<<std::endl;
    now_page = sys_page_mgr.fetch_page(fd, SmManager::db.tabs[tab_name].pages[now_pageid]);
    
    // 构造
    std::cout << "OK2!" << std::endl;
    page_handle = new RmFixLenPageHandle(now_page,bmp_size,rec_size);
    std::cout << "OK3!" << std::endl;
    page_scan = new RmFixLenScan(page_handle);

    std::cout << "scanner created" << std::endl;
}

QlFixLenScan::~QlFixLenScan(){
    if(!is_end()){
        delete page_scan;
        delete page_handle;
    }
    for(int i = 0; i < rec_raws.size(); i++){
        delete rec_raws[i];
    }
}

void QlFixLenScan::reset() {
    if(!is_end()){
        delete page_scan;
        delete page_handle;
    }
    now_pageid = 0;
    now_id = -1;
    now_page = sys_page_mgr.fetch_page(fd, SmManager::db.tabs[tab_name].pages[now_pageid]);
    page_handle = new RmFixLenPageHandle(now_page,bmp_size,rec_size);
    page_scan = new RmFixLenScan(page_handle);
}

bool QlFixLenScan::next() {
    // 当前页下一条记录
    
    page_scan->next();
    std::cout << "99OK!" << std::endl;
    // 如果到页的结尾，开新页
    while(page_scan->is_end()){
        delete page_scan;
        delete page_handle;
        now_pageid++;
        // 遍历结束
        if(is_end()){
            return false;
        }
        now_page = sys_page_mgr.fetch_page(fd, SmManager::db.tabs[tab_name].pages[now_pageid]);
        page_handle = new RmFixLenPageHandle(now_page,bmp_size,rec_size);
        page_scan = new RmFixLenScan(page_handle);
        page_scan->next();
    }
    now_id = page_scan->now_id;
    // 获取记录
    std::unique_ptr<FixLenRecord> raw = page_handle->get_record(now_id);
    std::cout << "100OK!" << std::endl;
    rec_header = raw->data[0];
    // 读取是否为空
    for(int i = 0; i < col_size; i++){
        rec_raws[i]->is_null = Bitmap::test(raw->data+1, i);
    }
    // 读取记录
    int start_pos = 1 + (col_size+7)/8;  // 当前原始数据开始位置
    for(int i = 0; i < rec_raws.size(); i++){
        if(rec_raws[i]->is_null)
            continue;
        int tmplen = rec_raws[i]->len;
        for(int j = 0; j < tmplen; j++){
            rec_raws[i]->raw[j] = raw->data[start_pos];
            start_pos++;
        }
    }
    return true;
}

bool QlFixLenScan::first_free_page(){
    // 重置
    this->reset();
    // 找到第一页未满的页
    while(page_handle->is_full()){
        delete page_scan;
        delete page_handle;
        now_pageid++;
        // 遍历结束
        if(is_end()){
            return false;
        }
        now_page = sys_page_mgr.fetch_page(fd, SmManager::db.tabs[tab_name].pages[now_pageid]);
        page_handle = new RmFixLenPageHandle(now_page,bmp_size,rec_size);
        page_scan = new RmFixLenScan(page_handle);
    }
    now_id = page_scan->now_id;
    return true;
}