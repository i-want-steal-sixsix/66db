#ifndef SM_METE_H
#define SM_METE_H

#include "sm_def.h"
#include "../defs.h"
#include "../error.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct ColMeta {
    std::string name;
    uint8_t type;
    uint16_t len;
    uint8_t option;

    ColMeta() = default;
    ColMeta(std::string name_, uint8_t type_, uint16_t len_, uint8_t option_)
    : name(std::move(name_)), type(type_), len(len_), option(option_){}

    friend std::ostream &operator<<(std::ostream &os, const ColMeta &col) {
        return os << "column name: " << col.name << " datatype: " << int(col.type) << " length: " << col.len << " option: " << int(col.option);
    }

    // <列名> <数据类型> <长度> <约束>
    friend std::istream &operator>>(std::istream &is, ColMeta &col) {
        int tmptype;
        return is >> col.name >> tmptype >> col.len >> col.option;
        col.type = ColType(tmptype);
    }
};

struct TabMeta {
    Rid rid;
    std::string name;
    std::vector<ColMeta> cols;
    std::vector<uint16_t> pages;

    bool is_col(const std::string &col_name){
        std::vector<ColMeta>::iterator pos = cols.begin();
        for(; pos != cols.end(); pos++){if(pos->name == col_name) break;}
        return pos != cols.end();
    }

    std::vector<ColMeta>::iterator get_col(const std::string &col_name) {
        std::vector<ColMeta>::iterator pos = cols.begin();
        for(; pos != cols.end(); pos++){if(pos->name == col_name) break;}
        if (pos == cols.end()) {
            throw ColumnNotFoundError(col_name);
        }
        return pos;
    }

    friend std::ostream &operator<<(std::ostream &os, const TabMeta &tab) {
        os << "table name: " << tab.name << " column count: " << tab.cols.size() << std::endl;
        for (const ColMeta &col : tab.cols) {
            os << col << std::endl;
        }
        return os;
    }

    // <表名> <列个数> <列>,... <数据页数> <页编号>,...
    friend std::istream &operator>>(std::istream &is, TabMeta &tab) {
        int colCount, pageCount;
        is >> tab.name >> colCount;
        for (int i = 0; i < colCount; i++) {
            ColMeta col;
            is >> col;
            tab.cols.push_back(col);
        }
        is >> pageCount;
        for (int i = 0; i < pageCount; i++) {
            uint16_t pageid;
            is >> pageid;
            tab.pages.push_back(pageid);
        }
        return is;
    }
};

struct DbMeta {
    std::string name;
    std::map<std::string, TabMeta> tabs;

    bool is_table(const std::string &tab_name) const { return tabs.find(tab_name) != tabs.end(); }

    TabMeta &get_table(const std::string &tab_name) {
        std::map<std::string, TabMeta>::iterator pos = tabs.find(tab_name);
        if (pos == tabs.end()) {
            throw TableNotFoundError(tab_name);
        }
        return pos->second;
    }

    friend std::ostream &operator<<(std::ostream &os, const DbMeta &db_meta) {
        os << "Database: " << db_meta.name << " table count: " << db_meta.tabs.size() << std::endl << std::endl;
        for (const std::pair<std::string, TabMeta> &tab : db_meta.tabs) {
            os << tab.second << std::endl;
        }
        return os;
    }

    // <数据库名> <表1>,...
    friend std::istream &operator>>(std::istream &is, DbMeta &db_meta) {
        int n;
        is >> db_meta.name >> n;
        for (int i = 0; i < n; i++) {
            TabMeta tab;
            is >> tab;
            db_meta.tabs[tab.name] = tab;
        }
        return is;
    }
};

#endif