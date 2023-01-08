#pragma once

#include "../defs.h"
#include <string>
#include <string.h>

static const std::string DB_BASE_DIR = ".\\data\\";
static const std::string DB_DBF = ".dbf";
static const std::string DB_DAT = ".dat";

static const uint8_t DB_MAX_NAME_LENGTH = 32;
static const uint8_t TAB_MAX_NAME_LENGTH = 32;
static const uint16_t DB_MAX_TABLE_COUNT = 65535;
static const uint16_t DB_MAX_PAGE_COUNT = 65535;

static const int DBF_HEADER_SIZE = 96;
static const int DBF_TABIDX_BMP_SIZE = 96;
static const int DBF_STRUCT_BMP_SIZE = 96;
static const int DBF_DATIDX_BMP_SIZE = 960;
static const int DBF_TABIDX_REC_SIZE = 40;
static const int DBF_STRUCT_REC_SIZE = 40;
static const int DBF_DATIDX_REC_SIZE = 4;

enum DbfPageType{
    DBF_TABLEINDEX_PAGE,
    DBF_STRUCTURE_PAGE,
    DBF_DATAINDEX_PAGE
};

struct DatFileHdr {
    uint8_t undefined[34];
    // 2 bytes
    uint16_t next_free;

    DatFileHdr(uint16_t next_free_)
    :next_free(next_free_){
        memset(undefined,0,sizeof(undefined));
    }

}__attribute__((packed));

struct DbfFileHdr {
    char dbname[32];
    uint16_t table_count;
    uint16_t next_free;

    DbfFileHdr(std::string &dbname_, uint16_t table_count_, uint16_t next_free_)
    :table_count(table_count_),next_free(next_free_){
        int i = 0;
        for(; i < dbname_.size(); i++){
            dbname[i] = dbname_[i];
        }
        for(; i <= 31; i++){
            dbname[i] = 0;
        }
    }

}__attribute__((packed));

struct DbfTabIdxPageHdr {
    // 4 Bytes
    uint8_t page_type;
    uint16_t next_page;
    uint8_t record_count;
    // 92 Bytes
    uint8_t undefined[92];
    // bitmap (96bit)
    uint8_t bitmap[DBF_TABIDX_BMP_SIZE/8];

    DbfTabIdxPageHdr(){
        page_type = DBF_TABLEINDEX_PAGE;
        next_page = 0; record_count = 0;
        memset(undefined,0,sizeof(undefined));
        Bitmap::init(bitmap,sizeof(bitmap));
    }

}__attribute__((packed));

struct DbfStructPageHdr {
    // 4 Bytes
    uint8_t page_type;
    uint16_t next_page;
    uint8_t record_count;
    // 92 Bytes
    uint8_t undefined[92];
    // bitmap (96bit)
    uint8_t bitmap[DBF_STRUCT_BMP_SIZE/8];

    DbfStructPageHdr(){
        page_type = DBF_STRUCTURE_PAGE;
        next_page = 0; record_count = 0;
        memset(undefined,0,sizeof(undefined));
        Bitmap::init(bitmap,sizeof(bitmap));
    }

}__attribute__((packed));

struct DbfDatIdxPageHdr {
    // 5 Bytes
    uint8_t page_type;
    uint16_t next_page;
    uint16_t record_count;
    // 91 Bytes
    uint8_t undefined[91];
    // bitmap (960bit)
    uint8_t bitmap[DBF_DATIDX_BMP_SIZE/8];

    DbfDatIdxPageHdr(){
        page_type = DBF_DATAINDEX_PAGE;
        next_page = 0; record_count = 0;
        memset(undefined,0,sizeof(undefined));
        Bitmap::init(bitmap,sizeof(bitmap));
    }

}__attribute__((packed));

struct DbfTabIdxRec {
    // 36 Bytes
    char tabname[32];
    uint16_t structidx;
    uint16_t dataidx;
    // 4 Bytes
    uint8_t undefined[4];

    DbfTabIdxRec(std::string &tabname_, uint16_t structidx_, uint16_t dataidx_)
    :structidx(structidx_),dataidx(dataidx_){
        int i = 0;
        for(; i < tabname_.size(); i++){
            tabname[i] = tabname_[i];
        }
        for(; i <= 31; i++){
            tabname[i] = 0;
        }
        memset(undefined,0,sizeof(undefined));
    }
}__attribute__((packed));

struct DbfStructRec {
    // 36 Bytes
    char colname[32];
    uint8_t datatype;
    uint16_t length;
    uint8_t option;
    // 4 Bytes
    uint8_t undefined[4];

    DbfStructRec(std::string &colname_, uint8_t datatype_, uint16_t length_, uint16_t option_)
    :datatype(datatype_), length(length_),option(option_){
        int i = 0;
        for(; i < colname_.size(); i++){
            colname[i] = colname_[i];
        }
        for(; i <= 31; i++){
            colname[i] = 0;
        }
        memset(undefined,0,sizeof(undefined));
    }
}__attribute__((packed));

struct DbfDatIdxRec {
    // 2 Bytes
    uint16_t page_no;
    // 2 Bytes
    uint8_t undefined[2];

    DbfDatIdxRec(uint16_t page_no_)
    :page_no(page_no_){
        memset(undefined,0,sizeof(undefined));
    }

}__attribute__((packed));