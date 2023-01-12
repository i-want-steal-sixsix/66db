#pragma once

#include "ql_scan.h"
#include "ql_manager.h"
#include "../sqlparser/ast.h"
#include "../printer.h"
#include <vector>
#include <stack>
#include <map>
#include <exception>

void QlManager::insert_into(std::string tab_name, std::vector<Values*> values){

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
        if(values[i]->is_null){
            Bitmap::set(new_raw+1, i);
        }
        else{
            Bitmap::reset(new_raw+1, i);
        }
    }

    // 原始记录
    int startpos = 1 + (scanner.col_size+7)/8;
    for(int i = 0; i < values.size(); i++){
        if(values[i]->is_null)
            continue;
        switch(SmManager::db.tabs[tab_name].cols[i].type){
            case COL_TYPE_INT:
                if(values[i]->type == COL_TYPE_CHAR){
                    std::cout << "[ERROR] Invalid value." << std::endl;
                    sys_page_mgr.flush_file(fd);
                    PfFileManager::close_file(fd);
                    return;
                }
                if(values[i]->type == COL_TYPE_FLOAT){
                    values[i]->int_val = (int)(values[i]->float_val);
                }
                *((int*)(new_raw+startpos)) = values[i]->int_val;
                break;
            case COL_TYPE_FLOAT:
                if(values[i]->type == COL_TYPE_CHAR){
                    std::cout << "[ERROR] Invalid value." << std::endl;
                    sys_page_mgr.flush_file(fd);
                    PfFileManager::close_file(fd);
                    return;
                }
                if(values[i]->type == COL_TYPE_INT){
                    values[i]->float_val = (float)(values[i]->int_val);
                }
                *((float*)(new_raw+startpos)) = values[i]->float_val;
                break;
            case COL_TYPE_CHAR:
                if(values[i]->type != COL_TYPE_CHAR){
                    std::cout << "[ERROR] Invalid value." << std::endl;
                    sys_page_mgr.flush_file(fd);
                    PfFileManager::close_file(fd);
                    return;
                }
                str2char(new_raw+startpos, values[i]->length, values[i]->char_val);
                break;
            default:
                break;
        }
        startpos += SmManager::db.tabs[tab_name].cols[i].len;
    }
    

    // 释放values
    for(int i = 0; i < values.size(); i++){
        delete values[i];
    }

    // 保存
    scanner.page_handle->insert_record(new_raw);

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    //std::cout << "insert OK" << std::endl;

    return;
}

void QlManager::delete_from(const std::string &tab_name, inter_w conds){
    
    // 打开dat文件
    int fd = PfFileManager::open_file(DB_BASE_DIR+SmManager::db.name+DB_DAT);

    // 创建列名对应表以及类型
    int nowpos = 0;
    std::map<std::string, QlColIdx> col_idx;
    for(int j = 0; j < SmManager::db.tabs[tab_name].cols.size(); j++){
        std::string tmpcol = tab_name + "." + SmManager::db.tabs[tab_name].cols[j].name;
        col_idx[tmpcol] = {nowpos, SmManager::db.tabs[tab_name].cols[j].type};
        nowpos++;
    }

    //std::cout << "col_idx map OK!" << std::endl;
    // for(auto &x: col_idx){
    //     std::cout << "key: " << x.first << " value: " << x.second.idx << " , " << x.second.typ << std::endl;
    // }

    // 创建QlScanner
    QlFixLenScan *scanner = new QlFixLenScan(fd, tab_name);

    // 遍历每一条记录
    std::vector<RecordRaw*> tmprec;
    while(scanner->next()){
        tmprec.clear();

        for(int j = 0; j < scanner->col_size; j++){
            RecordRaw *ttmp = new RecordRaw(scanner->rec_raws[j]->len);
            ttmp->is_null = scanner->rec_raws[j]->is_null;
            for(int t = 0; t < scanner->rec_raws[j]->len; t++){
                ttmp->raw[t] = scanner->rec_raws[j]->raw[t];
            }
            tmprec.push_back(ttmp);
        }

        Values tmpval = where_judge(conds, tmprec, col_idx);
        switch(tmpval.type){
            case COL_TYPE_INT:
                if(tmpval.int_val != 0) scanner->page_handle->delete_record(scanner->now_id);
                break;
            case COL_TYPE_FLOAT:
                if(tmpval.float_val != 0) scanner->page_handle->delete_record(scanner->now_id);
                break;
            case COL_TYPE_CHAR:
                if(tmpval.char_val.size() > 0) scanner->page_handle->delete_record(scanner->now_id);
                break;
            default:
                sys_page_mgr.flush_file(fd);
                PfFileManager::close_file(fd);
                return;
        }

    }

    // 删除QlScanner
    delete scanner;
    
    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    return;
}

void QlManager::select_from(std::vector<SelColMeta> sel_cols, std::vector<SelTabMeta> &tab_names, inter_w conds){
    std::vector<std::vector<std::string>> result;

    // 打开dat文件
    int fd = PfFileManager::open_file(DB_BASE_DIR+SmManager::db.name+DB_DAT);

    // 创建别名对应表
    std::map<std::string, std::string> alt_tab;
    for(int i = 0; i < tab_names.size(); i++){
        alt_tab[tab_names[i].alt_name] = tab_names[i].tab_name;
    }
    //std::cout << "alt_tab map OK!" << std::endl;

    // 创建列名对应表以及类型
    int nowpos = 0;
    std::map<std::string, QlColIdx> col_idx;
    for(int i = 0; i < tab_names.size(); i++){
        std::string tab_name = tab_names[i].tab_name;
        for(int j = 0; j < SmManager::db.tabs[tab_name].cols.size(); j++){
            std::string tmpcol = tab_names[i].alt_name + "." + SmManager::db.tabs[tab_name].cols[j].name;
            col_idx[tmpcol] = {nowpos, SmManager::db.tabs[tab_name].cols[j].type};
            nowpos++;
        }
    }
    //std::cout << "col_idx map OK!" << std::endl;
    // for(auto &x: col_idx){
    //     std::cout << "key: " << x.first << " value: " << x.second.idx << " , " << x.second.typ << std::endl;
    // }

    // 创建QlScanner组 并检查是不是有空的表
    bool is_empty = false;
    std::vector<QlFixLenScan*> scanner_array;
    for(int i = 0; i < tab_names.size(); i++){
        QlFixLenScan *tmpscan = new QlFixLenScan(fd, tab_names[i].tab_name);
        scanner_array.push_back(tmpscan);
        scanner_array[i]->next();
        if(scanner_array[i]->is_end())
            is_empty = true;
            
    }

    // 类型表
    std::vector<uint8_t> sel_type;
    std::vector<std::string> sel_header;
    // 表头
    for(int i = 0; i < sel_cols.size(); i++){
        if(sel_cols[i].show_name.size() > 0){
            sel_header.push_back(sel_cols[i].show_name);
        }
        else{
            std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
            sel_header.push_back(col_name);
        }
        
        std::string real_name = alt_tab[sel_cols[i].tab_name];
        sel_type.push_back(SmManager::db.tabs[real_name].get_col(sel_cols[i].col_name)->type);
    }
    //std::cout << "header: " << sel_header.size() << std::endl;
    result.push_back(sel_header);

    // 目前存在空表
    if(is_empty){
        printer::print_records(result);
        PfFileManager::close_file(fd);
        return;
    }

    // 笛卡尔积
    std::vector<std::vector<RecordRaw*>> sel_result;
    std::vector<RecordRaw*> tmprec;
    while(1){
        tmprec.clear();
        // 合成记录
        for(int i = 0; i < scanner_array.size(); i++){
            for(int j = 0; j < scanner_array[i]->col_size; j++){
                RecordRaw *ttmp = new RecordRaw(scanner_array[i]->rec_raws[j]->len);

                ttmp->is_null = scanner_array[i]->rec_raws[j]->is_null;

                //std::cout << "raw len: " << scanner_array[i]->rec_raws[j]->len << std::endl;

                for(int t = 0; t < scanner_array[i]->rec_raws[j]->len; t++){
                    ttmp->raw[t] = scanner_array[i]->rec_raws[j]->raw[t];
                    // std::cout << (int)(ttmp->raw[t]) << " ";

                }
                //std::cout    << std::endl;
                tmprec.push_back(ttmp);
            }
        }

        // std::cout << "combine" << std::endl;

        // WHERE 判断
        std::vector<RecordRaw*> tmpresrec;

        Values tmpval = where_judge(conds, tmprec, col_idx);
        switch(tmpval.type){
            case COL_TYPE_INT:
                if(tmpval.int_val != 0){
                    // 投影运算
                    for(int i = 0; i < sel_cols.size(); i++){
                        std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
                        int pos = col_idx[col_name].idx;
                        tmpresrec.push_back(tmprec[pos]);
                    }
                    sel_result.push_back(tmpresrec);
                }
                break;
            case COL_TYPE_FLOAT:
                if(tmpval.float_val != 0){
                    // 投影运算
                    for(int i = 0; i < sel_cols.size(); i++){
                        std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
                        int pos = col_idx[col_name].idx;
                        tmpresrec.push_back(tmprec[pos]);
                    }
                    sel_result.push_back(tmpresrec);
                }
                break;
            case COL_TYPE_CHAR:
                if(tmpval.char_val.size() > 0){
                    // 投影运算
                    for(int i = 0; i < sel_cols.size(); i++){
                        std::string col_name = sel_cols[i].tab_name + "." + sel_cols[i].col_name;
                        int pos = col_idx[col_name].idx;
                        tmpresrec.push_back(tmprec[pos]);
                    }
                    sel_result.push_back(tmpresrec);
                }
                break;
            default:
                sys_page_mgr.flush_file(fd);
                PfFileManager::close_file(fd);
                return;
        }

        // 下一条记录
        int i = 0;
        for(; i < scanner_array.size(); i++){
            scanner_array[i]->next();
            if(!scanner_array[i]->is_end())
                break;
            scanner_array[i]->reset();
            scanner_array[i]->next();
        }

        // 全部遍历完毕
        if(i == scanner_array.size())
            break;
    }

    // 输出结果
    // 表内容
    for(int i = 0; i < sel_result.size(); i++){
        std::vector<std::string> tmp_sel_rec;
        for(int j = 0; j < sel_cols.size(); j++){
            if(sel_result[i][j]->is_null){
                tmp_sel_rec.push_back("(NULL)");
                continue;
            }
            std::string tmpstr;
            switch(sel_type[j]) {
                case COL_TYPE_INT:
                    tmpstr = std::to_string(*((int*)sel_result[i][j]->raw));
                    break;
                case COL_TYPE_FLOAT:
                    tmpstr = std::to_string(*((float*)sel_result[i][j]->raw));
                    break;
                case COL_TYPE_CHAR:
                    tmpstr = char2str(sel_result[i][j]->raw, sel_result[i][j]->len);
                    break;
                default:
                    break;
            }
            // std::cout << "string: " << tmpstr << std::endl;
            tmp_sel_rec.push_back(tmpstr);
        }
        result.push_back(tmp_sel_rec);
    }

    for(int i = 0; i < scanner_array.size(); i++){
        delete scanner_array[i];
    }

    printer::print_records(result);

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    return;
}

void QlManager::update_set(const std::string &tab_name, std::vector<std::string> set_cols, std::vector<inter_w*> set_exprs, inter_w conds){

    /* debug
    std::cout << "tab_name: ";
    std::cout << tab_name << std::endl;
    std::cout << "set_cols: " << std::endl;
    for(auto &x : set_cols){
        std::cout << x << std::endl;
    }
    std::cout << "set_exprs: " << std::endl;
    for(auto &x : set_exprs){
        std::cout << "vec1: " << std::endl;
        for(auto &s : x->vec1){
            std::cout << s << "  ";
        }
        std::cout << "\nvec2: " << std::endl;
        for(auto &s : x->vec2){
            std::cout << s << "  ";
        }
    }
    std::cout << "conds: " << std::endl;
    for(auto &s : conds.vec1){
        std::cout << s << "  ";
    }
    std::cout << "\nvec2: " << std::endl;
    for(auto &s : conds.vec2){
        std::cout << s << "  ";
    }
    */
    
    // 打开dat文件
    int fd = PfFileManager::open_file(DB_BASE_DIR+SmManager::db.name+DB_DAT);

    // 创建列名对应表以及类型
    int nowpos = 0;
    std::map<std::string, QlColIdx> col_idx;
    for(int j = 0; j < SmManager::db.tabs[tab_name].cols.size(); j++){
        std::string tmpcol = tab_name + "." + SmManager::db.tabs[tab_name].cols[j].name;
        col_idx[tmpcol] = {nowpos, SmManager::db.tabs[tab_name].cols[j].type};
        nowpos++;
    }

    //std::cout << "col_idx map OK!" << std::endl;
    // for(auto &x: col_idx){
    //     std::cout << "key: " << x.first << " value: " << x.second.idx << " , " << x.second.typ << std::endl;
    // }

    // 创建更新表的对应列
    std::map<int, int> idx_updcol;
    for(int j = 0; j < set_cols.size(); j++){
        idx_updcol[col_idx[set_cols[j]].idx] = j;
    }

    // 创建QlScanner
    QlFixLenScan *scanner = new QlFixLenScan(fd, tab_name);

    // 遍历每一条记录
    std::vector<RecordRaw*> tmprec;
    while(scanner->next()){
        tmprec.clear();

        for(int j = 0; j < scanner->col_size; j++){
            RecordRaw *ttmp = new RecordRaw(scanner->rec_raws[j]->len);
            ttmp->is_null = scanner->rec_raws[j]->is_null;
            for(int t = 0; t < scanner->rec_raws[j]->len; t++){
                ttmp->raw[t] = scanner->rec_raws[j]->raw[t];
            }
            tmprec.push_back(ttmp);
        }

        Values tmpval = where_judge(conds, tmprec, col_idx);
        bool update_flag = false;
        switch(tmpval.type){
            case COL_TYPE_INT:
                if(tmpval.int_val != 0) update_flag = true;
                break;
            case COL_TYPE_FLOAT:
                if(tmpval.float_val != 0) update_flag = true;
                break;
            case COL_TYPE_CHAR:
                if(tmpval.char_val.size() > 0) update_flag = true;
                break;
            default:
                sys_page_mgr.flush_file(fd);
                PfFileManager::close_file(fd);
                return;
        }

        // 更新
        if(update_flag){
            // 新纪录
            uint8_t *newbuf = new uint8_t[scanner->rec_size];
            int buf_ptr = 1 + (scanner->col_size+7)/8;

            // 目前记录头无效并为 0
            newbuf[0] = 0;

            // 先写原记录再写更改
            for(int i = 0; i < SmManager::db.tabs[tab_name].cols.size(); i++){
                // std::cout << "buf_ptr: " << buf_ptr << std::endl;
                // 旧值
                if(tmprec[i]->is_null){
                    Bitmap::set(newbuf+1, i);
                }
                else{
                    Bitmap::reset(newbuf+1, i);
                    memcpy(newbuf+buf_ptr, tmprec[i]->raw, tmprec[i]->len);
                }
                // 新值
                if(idx_updcol.count(i) > 0){
                    Values new_val = where_judge(*set_exprs[idx_updcol[i]], tmprec, col_idx);
                    // std::cout << " " << new_val.int_val << std::endl;
                    Bitmap::reset(newbuf+1, i);
                    switch(SmManager::db.tabs[tab_name].cols[i].type){
                        case COL_TYPE_INT:
                            if(new_val.type == COL_TYPE_CHAR){
                                std::cout << "[Error] Invalid SET Value." << std::endl;
                                sys_page_mgr.flush_file(fd);
                                PfFileManager::close_file(fd);
                                return;
                            }
                            if(new_val.type == COL_TYPE_FLOAT){
                                new_val.int_val = (int)(new_val.float_val);
                            }
                            memcpy(newbuf+buf_ptr, (uint8_t*)&(new_val.int_val), tmprec[i]->len);
                            break;
                        case COL_TYPE_FLOAT:
                            if(new_val.type == COL_TYPE_CHAR){
                                std::cout << "[Error] Invalid SET Value." << std::endl;
                                sys_page_mgr.flush_file(fd);
                                PfFileManager::close_file(fd);
                                return;
                            }
                            if(new_val.type == COL_TYPE_INT){
                                new_val.float_val = (float)(new_val.int_val);
                            }
                            memcpy(newbuf+buf_ptr, (uint8_t*)&(new_val.float_val), tmprec[i]->len);
                            break;
                        case COL_TYPE_CHAR:
                            if(new_val.type != COL_TYPE_CHAR){
                                std::cout << "[Error] Invalid SET Value." << std::endl;
                                sys_page_mgr.flush_file(fd);
                                PfFileManager::close_file(fd);
                                return;
                            }
                            str2char(newbuf+buf_ptr, tmprec[i]->len, new_val.char_val);
                            break;
                        default:
                            sys_page_mgr.flush_file(fd);
                            PfFileManager::close_file(fd);
                            return;
                    }
                }
                buf_ptr += tmprec[i]->len;
            }
            scanner->page_handle->update_record(scanner->now_id, newbuf);
            delete newbuf;
        }

    }

    // 删除QlScanner
    delete scanner;
    
    for(int i = 0; i < set_exprs.size(); i++){
        delete set_exprs[i];        
    }

    sys_page_mgr.flush_file(fd);

    PfFileManager::close_file(fd);

    return;
}

Values QlManager::where_judge(inter_w condition, std::vector<RecordRaw*>&rec, std::map<std::string, QlColIdx> &col_idx){
    std::vector<std::string> tmp_val;
    std::vector<int> tmp_type;
    for(int i = 0; i < condition.vec1.size(); i++){
        //std::cout << "entering" << std::endl;
        // 压栈
        tmp_val.push_back(condition.vec1[i]);
        tmp_type.push_back(condition.vec2[i]);
        // 符号直接压栈
        if(condition.vec2[i] == EXPR_TYPE_OPERATOR){
            //std::cout << "push operator" << std::endl;
            continue;
        }
        // 前一项不是值，压栈
        else if(tmp_type.size() == 1 || tmp_type[tmp_type.size()-2] == EXPR_TYPE_OPERATOR){
            //std::cout << "push no num" << std::endl;
            continue;
        }
        // 前一项是值，计算
        else{
            while(tmp_type.size() > 1 && (tmp_type[tmp_type.size()-2] != EXPR_TYPE_OPERATOR)){
                //std::cout << "calculate" << std::endl;
                Values *lval, *rval;
                // 左值
                int idx, typ;
                uint16_t len;
                switch(tmp_type[tmp_type.size()-2]){
                    case EXPR_TYPE_INT:
                        lval = new Values(COL_TYPE_INT, 4, false);
                        lval->int_val = atoi(tmp_val[tmp_type.size()-2].c_str());
                        break;
                    case EXPR_TYPE_FLOAT:
                        lval = new Values(COL_TYPE_FLOAT, 4, false);
                        lval->float_val = atof(tmp_val[tmp_type.size()-2].c_str());
                        break;
                    case EXPR_TYPE_CHAR:
                        lval = new Values(COL_TYPE_CHAR, tmp_val[tmp_type.size()-2].size(), false);
                        lval->char_val = tmp_val[tmp_type.size()-2];
                        break;
                    case EXPR_TYPE_COLUMN:
                       // std::cout << "col: " << tmp_val[tmp_type.size()-2] << std::endl;
                        idx = col_idx[tmp_val[tmp_type.size()-2]].idx;
                        len = rec[idx]->len;
                        typ = col_idx[tmp_val[tmp_type.size()-2]].typ;
                        lval = new Values(typ, len, false);
                        switch(typ){
                            case COL_TYPE_INT:
                                lval->int_val = *((int *)rec[idx]->raw);
                                break;
                            case COL_TYPE_FLOAT:
                                lval->float_val = *((float *)rec[idx]->raw);
                                break;
                            case COL_TYPE_CHAR:
                                lval->char_val = char2str(rec[idx]->raw, rec[idx]->len);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        std::cout << "Invalid type" << std::endl;
                        break;
                }
                // 右值
                switch(tmp_type.back()){
                    case EXPR_TYPE_INT:
                        rval = new Values(COL_TYPE_INT, 4, false);
                        rval->int_val = atoi(tmp_val.back().c_str());
                        break;
                    case EXPR_TYPE_FLOAT:
                        rval = new Values(COL_TYPE_FLOAT, 4, false);
                        rval->float_val = atof(tmp_val.back().c_str());
                        break;
                    case EXPR_TYPE_CHAR:
                        rval = new Values(COL_TYPE_CHAR, tmp_val.back().size(), false);
                        rval->char_val = tmp_val.back();
                        break;
                    case EXPR_TYPE_COLUMN:
                        idx = col_idx[tmp_val.back()].idx;
                        len = rec[idx]->len;
                        typ = col_idx[tmp_val.back()].typ;
                        rval = new Values(typ, len, false);
                        switch(typ){
                            case COL_TYPE_INT:
                                rval->int_val = *((int *)rec[idx]->raw);
                                break;
                            case COL_TYPE_FLOAT:
                                rval->float_val = *((float *)rec[idx]->raw);
                                break;
                            case COL_TYPE_CHAR:
                                rval->char_val = char2str(rec[idx]->raw, rec[idx]->len);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        std::cout << "Invalid type" << std::endl;
                        break;
                }
                tmp_val.pop_back(); tmp_type.pop_back();
                tmp_val.pop_back(); tmp_type.pop_back();

                // debug
                /* // std::cout << "lval: " << std::endl;
                switch(lval->type){
                    case COL_TYPE_INT:
                        // std::cout << lval->int_val << std::endl; 
                        break;
                    case COL_TYPE_FLOAT:
                        // std::cout << lval->float_val << std::endl; 
                        break;
                    case COL_TYPE_CHAR:
                        std::cout << lval->char_val << std::endl; 
                        break;
                    default:
                        break;
                }
                // std::cout << "rval: " << std::endl;
                switch(rval->type){
                    case COL_TYPE_INT:
                        //std::cout << rval->int_val << std::endl; break;
                    case COL_TYPE_FLOAT:
                        std::cout << rval->float_val << std::endl; break;
                    case COL_TYPE_CHAR:
                        std::cout << rval->char_val << std::endl; break;
                    default:
                        break;
                }
                // */

                // 计算
                std::string res_val;
                int res_type;
                bool lbool, rbool;
                switch(atoi(tmp_val.back().c_str())){
                    // 算术运算
                    case ast::EXP_OP_ADD:
                        if(lval->type==COL_TYPE_CHAR||rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->int_val + rval->int_val);
                            res_type = EXPR_TYPE_INT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->float_val + rval->int_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->int_val + rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->float_val + rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        break;
                    case ast::EXP_OP_SUB:
                        if(lval->type==COL_TYPE_CHAR||rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->int_val - rval->int_val);
                            res_type = EXPR_TYPE_INT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->float_val - rval->int_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->int_val - rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->float_val - rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        break;
                    case ast::EXP_OP_MUL:
                        if(lval->type==COL_TYPE_CHAR||rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->int_val * rval->int_val);
                            res_type = EXPR_TYPE_INT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->float_val * rval->int_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->int_val * rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->float_val * rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        break;
                    case ast::EXP_OP_DIV:
                        if(lval->type==COL_TYPE_CHAR||rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->int_val / rval->int_val);
                            res_type = EXPR_TYPE_INT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string(lval->float_val / rval->int_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->int_val / rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string(lval->float_val / rval->float_val);
                            res_type = EXPR_TYPE_FLOAT;
                        }
                        break;
                    // 比较运算
                    case ast::EXP_OP_EQU:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val==rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val == rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val == rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val == rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val == rval->float_val));
                        }
                        break;
                    case ast::EXP_OP_NEQ:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val!=rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val != rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val != rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val != rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val != rval->float_val));
                        }
                        break;
                    case ast::EXP_OP_L:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val < rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val < rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val < rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val < rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val < rval->float_val));
                        }
                        break;
                    case ast::EXP_OP_LE:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val <= rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val <= rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val <= rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val <= rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val <= rval->float_val));
                        }
                        break;
                    case ast::EXP_OP_G:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val > rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val > rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val > rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val > rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val > rval->float_val));
                        }
                        break;
                    case ast::EXP_OP_GE:
                        res_type = EXPR_TYPE_INT;
                        if(lval->type==COL_TYPE_CHAR^rval->type==COL_TYPE_CHAR){
                            std::cout << "[Error] Invalid Expression." << std::endl;
                            return Values(COL_TYPE_ERROR, 0, true);
                        }
                        if(lval->type==COL_TYPE_CHAR&&rval->type==COL_TYPE_CHAR){
                            res_val = std::to_string((int)(lval->char_val >= rval->char_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->int_val >= rval->int_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_INT){
                            res_val = std::to_string((int)(lval->float_val >= rval->int_val));
                        }
                        if(lval->type==COL_TYPE_INT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->int_val >= rval->float_val));
                        }
                        if(lval->type==COL_TYPE_FLOAT&&rval->type==COL_TYPE_FLOAT){
                            res_val = std::to_string((int)(lval->float_val >= rval->float_val));
                        }
                        break;
                    // 逻辑运算符
                    case ast::EXP_OP_AND:
                        res_type = EXPR_TYPE_INT;
                        switch(lval->type){
                            case COL_TYPE_INT:
                                lbool = (lval->int_val != 0);
                                break;
                            case COL_TYPE_FLOAT:
                                lbool = (lval->float_val != 0);
                                break;
                            case COL_TYPE_CHAR:
                                lbool = (lval->char_val.size() != 0);
                                break;
                            default:
                                break;
                        }
                        switch(rval->type){
                            case COL_TYPE_INT:
                                rbool = (rval->int_val != 0);
                                break;
                            case COL_TYPE_FLOAT:
                                rbool = (rval->float_val != 0);
                                break;
                            case COL_TYPE_CHAR:
                                rbool = (rval->char_val.size() != 0);
                                break;
                            default:
                                break;
                        }
                        res_val = std::to_string((int)(lbool && rbool));
                        break;
                    case ast::EXP_OP_OR:
                        res_type = EXPR_TYPE_INT;
                        switch(lval->type){
                            case COL_TYPE_INT:
                                lbool = (lval->int_val != 0);
                                break;
                            case COL_TYPE_FLOAT:
                                lbool = (lval->float_val != 0);
                                break;
                            case COL_TYPE_CHAR:
                                lbool = (lval->char_val.size() != 0);
                                break;
                            default:
                                break;
                        }
                        switch(rval->type){
                            case COL_TYPE_INT:
                                rbool = (rval->int_val != 0);
                                break;
                            case COL_TYPE_FLOAT:
                                rbool = (rval->float_val != 0);
                                break;
                            case COL_TYPE_CHAR:
                                rbool = (rval->char_val.size() != 0);
                                break;
                            default:
                                break;
                        }
                        res_val = std::to_string((int)(lbool || rbool));
                        break;
                    default:
                        break;
                }
                tmp_val.pop_back(); tmp_type.pop_back();
                tmp_val.push_back(res_val); tmp_type.push_back(res_type);

                // std::cout << "\nres: " << res_val << std::endl;
                // std::cout << "\ntmp size: " << tmp_val.size() << std::endl;
            }
        }
    }
    // std::cout << "final res: " << tmp_val.back() << std::endl;
    Values result(COL_TYPE_ERROR,0,false);
    switch(tmp_type.back()){
        case EXPR_TYPE_INT:
            result.type = COL_TYPE_INT;
            result.length = 4;
            result.int_val = atoi(tmp_val.back().c_str());
            break;
        case EXPR_TYPE_FLOAT:
            result.type = COL_TYPE_FLOAT;
            result.length = 4;
            result.float_val = atof(tmp_val.back().c_str());
            break;
        case EXPR_TYPE_CHAR:
            result.type = COL_TYPE_CHAR;
            result.length = tmp_val.back().size();
            result.char_val = tmp_val.back();
            break;
        default:
            result.is_null = true;
            break;
    }
    return result; 
}