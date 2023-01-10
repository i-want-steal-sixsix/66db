#pragma once

#include "ql_scan.h"
#include "ql_manager.h"
#include <vector>
#include <map>

void QlManager::insert_into(std::string tab_name, std::vector<Values> values){

    // 打开dat文件
    int fd = PfFileManager::open_file(DB_BASE_DIR+SmManager::db.name+DB_DAT);

    // 创建 QlScanner
    QlFixLenScan scanner(fd, tab_name);

    // 尝试找空页
    while(!scanner.first_free_page()){
        // 创建新的 dat 页
        SmManager::add_data_page(fd, tab_name, scanner.rec_size);
    }

    // 转化记录
    uint8_t *new_raw = new uint8_t[scanner.rec_size];
    memset(new_raw, 0, scanner.rec_size);

    // 记录头（展示没有）

    // 空标记
    for(int i = 0; i < values.size(); i++){
        if(values[i].is_null){  
            Bitmap::set(new_raw+1, i);
        }
        else{
            Bitmap::reset(new_raw+1, i);
        }
    }

    // 原始记录
    int startpos = 1 + (scanner.col_size+7)/8;
    for(int i = 0; i < values.size(); i++){
        if(values[i].is_null)
            continue;
        switch(values[i].type){
            case COL_TYPE_INT:
                *((int*)(new_raw+startpos)) = values[i].int_val;
                break;
            case COL_TYPE_FLOAT:
                *((float*)(new_raw+startpos)) = values[i].float_val;
                break;
            case COL_TYPE_CHAR:
                str2char(new_raw+startpos, values[i].length, values[i].char_val);
                break;
            default:
                break;
        }
        startpos += values[i].length;
    }

    // 保存
    scanner.page_handle->insert_record(new_raw);

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    std::cout << "insert OK" << std::endl;

    return;
}


void QlManager::select_from(std::vector<SelColMeta> sel_cols, std::vector<SelTabMeta> &tab_names){

    // 打开dat文件
    int fd = PfFileManager::open_file(DB_BASE_DIR+SmManager::db.name+DB_DAT);

    // 创建别名对应表
    std::map<std::string, std::string> alt_tab;
    for(int i = 0; i < tab_names.size(); i++){
        alt_tab[tab_names[i].alt_name] = tab_names[i].tab_name;
    }
    std::cout << "alt_tab map OK!" << std::endl;

    // 创建列名对应表
    int nowpos = 0;
    std::map<std::string, int> col_idx;
    for(int i = 0; i < tab_names.size(); i++){
        std::string tab_name = tab_names[i].tab_name;
        for(int j = 0; j < SmManager::db.tabs[tab_name].cols.size(); j++){
            std::string tmpcol = tab_names[i].alt_name + "." + SmManager::db.tabs[tab_name].cols[j].name;
            col_idx[tmpcol] = nowpos;
            nowpos++;
        }
    }
    std::cout << "col_idx map OK!" << std::endl;
    for(std::map<std::string, int>::iterator iter = col_idx.begin(); iter != col_idx.end(); iter++){
        std::cout << "key: " << iter->first << " value: " << iter->second << std::endl;
    }

    // 创建QlScanner组 并检查是不是有空的表
    bool is_empty = false;
    std::vector<QlFixLenScan> scanner_array;
    for(int i = 0; i < tab_names.size(); i++){
        std::cout << "66OK!" << std::endl;
        scanner_array.push_back(QlFixLenScan(fd, tab_names[i].tab_name));
        std::cout << "77OK!" << std::endl;
        scanner_array[i].next();
        std::cout << "88OK!" << std::endl;
        if(scanner_array[i].is_end())
            is_empty = true;
            
    }
    std::cout << "scanner OK!" << std::endl;

// 目前存在空的情况先不管！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
    if(is_empty){
        std::cout << "One of the tables is EMPTY!" << std::endl;
        return;
    }

    // 笛卡尔积
    std::vector<std::vector<std::shared_ptr<RecordRaw>>> sel_result;
    while(1){
        std::vector<std::shared_ptr<RecordRaw>> tmprec;
        std::cout << "model created" << std::endl;
        // 合成记录
        for(int i = 0; i < scanner_array.size(); i++){
            std::cout << "56f" << std::endl;
            for(int j = 0; j < scanner_array[i].col_size; j++){
                std::cout << "mdf523525f" << std::endl;
                tmprec.push_back(std::make_shared<RecordRaw>(scanner_array[i].rec_raws[j]));
                std::cout << "m6767f" << std::endl;
            }
            std::cout << "mdfdfdf" << std::endl;
        }
        // WHERE 判断

        std::cout << "combine" << std::endl;

        // 投影运算
        std::vector<std::shared_ptr<RecordRaw>> tmpresrec;
        for(int i = 0; i < sel_cols.size(); i++){
            std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
            int pos = col_idx[col_name];
            tmpresrec.push_back(tmprec[pos]);
        }
        sel_result.push_back(tmpresrec);

        std::cout << "project" << std::endl;

        // 下一条记录
        int i = 0;
        for(; i < scanner_array.size(); i++){
            scanner_array[i].next();
            if(!scanner_array[i].is_end())
                break;
            scanner_array[i].reset();
            scanner_array[i].next();
        }

        std::cout << "next" << std::endl;

        // 全部遍历完毕
        if(i == scanner_array.size())
            break;

    }
    std::cout << "dicarl OK!" << std::endl;

    // 输出结果 （目前没有printer）
    // 类型表
    std::vector<uint8_t> sel_type;
    // 表头
    for(int i = 0; i < sel_cols.size(); i++){
        std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
        std::cout << col_name << "    ";
        
        std::string real_name = alt_tab[sel_cols[i].tab_name];
        sel_type.push_back(SmManager::db.tabs[real_name].get_col(sel_cols[i].col_name)->type);

    }
    std::cout << std::endl;
    // 表内容
    for(int i = 0; i < sel_result.size(); i++){
        for(int j = 0; j < sel_cols.size(); j++){
            if(sel_result[i][j]->is_null){
                std::cout << "(NULL)";
                continue;
            }
            std::string tmpstr;
            switch(sel_type[j]) {
                case COL_TYPE_INT:
                    std::cout << *((int*)sel_result[i][j]->raw);
                    break;
                case COL_TYPE_FLOAT:
                    std::cout << *((float*)sel_result[i][j]->raw);
                    break;
                case COL_TYPE_CHAR:
                    tmpstr = char2str(sel_result[i][j]->raw, sel_result[i][j]->len);
                    std::cout << tmpstr;
                    break;
                default:
                    break;
            }
            std::cout << "    ";
        }
        std::cout << std::endl;
    }

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    return;
}