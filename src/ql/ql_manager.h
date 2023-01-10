#pragma once

#include "ql_defs.h"
#include "../rm/rm_page_handle.h"
#include "../rm/rm_scan.h"
#include "../defs.h"
#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <string>
#include <vector>

class QlManager {
public:
    static void insert_into(std::string tab_name, std::vector<Values> values);

    //static void delete_from(const std::string &tab_name, std::vector<Condition> conds);

    //static void update_set(const std::string &tab_name, std::vector<SetClause> set_clauses,
    //                       std::vector<Condition> conds);

    static void select_from(std::vector<SelColMeta> sel_cols, std::vector<SelTabMeta> &tab_names);
    //                       std::vector<Condition> conds);

private:

    static std::string char2str(uint8_t *buf, uint16_t len){
        std::string res;
        for(int i = 0; i < len; i++){
            if(buf[i] == '\0')
                break;
            res.push_back((char)buf[i]);
        }
        return res;
    }

    static void str2char(uint8_t *buf, uint16_t len, std::string str){
        int i = 0;
        for(; i < len && i < str.size(); i++){
            buf[i] = str[i];
        }
        for(; i < len; i++){
            buf[i] = '\0';
        }
        return;
    }

};
