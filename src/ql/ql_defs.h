#pragma once

#include "../defs.h"
#include <string.h>

struct RecordRaw{
    uint16_t len;
    uint8_t *raw;
    bool is_null;

    RecordRaw(uint16_t rec_len){
        len = rec_len;
        try{
            raw = new uint8_t[rec_len];
            memset(raw, 0,rec_len);
        }
        catch(std::exception &e){
            std::cout << "exception1: "<< e.what() << std::endl;
        }
        is_null = false;
    }

    ~RecordRaw(){
        delete [] raw;
    }

};

struct QlColIdx{
    int idx;
    int typ;
};