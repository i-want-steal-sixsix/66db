#pragma once

#include "../sm/sm_manager.h"
#include "../rm/rm_scan.h"
#include "ql_defs.h"
#include "../pf/pf_fm.h"
#include "../pf/pf_pm.h"
#include <vector>
#include <string>
#include <iostream>

struct QlFixLenScan {

    QlFixLenScan(int fd_, std::string tab_name_);

    ~QlFixLenScan();

    void reset();

    bool next();
    
    bool first_free_page();

    bool is_end() {
        return now_pageid >= SmManager::db.tabs[tab_name].pages.size();
    }

    int fd;                         // dat 文件标识号

    std::string tab_name;           // 表名

// 拥有一个 RmPageHandle 和 RmFixLenScan
    RmFixLenPageHandle *page_handle;
    RmFixLenScan *page_scan;

// Scanner 当前位置
    Page* now_page;                 // 当前 page
    uint16_t now_pageid;            // 对应 page 下标
    uint16_t now_id;                // 对应当前 page 的记录号

// 调用 Rmscan 所需信息
    uint16_t bmp_size;              // bitmap长度
    uint16_t rec_size;              // 记录长度

// 分析原始记录所需信息
    uint16_t col_size;              // 列数

// 对应定长记录格式
    uint8_t rec_header;                 // 消息头
    std::vector<RecordRaw*> rec_raws;    // 原始记录 

};