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
            if(raw == NULL){
                std::cout << "WTF1" << std::endl; 
            }
            memset(raw, 0,rec_len);
        }
        catch(std::exception &e){
            std::cout << "exception1: "<< e.what() << std::endl;
        }
        is_null = false;
    }

    RecordRaw(RecordRaw *rec){
        len = rec->len;
        is_null = rec->is_null;
        try{
            raw = new uint8_t[len];
            if(raw == NULL){
                std::cout << "WTF2" << std::endl; 
            }
        }
        catch(std::exception &e){
            std::cout << "exception2: "<< e.what() << std::endl;
        }
        for(int i = 0; i < len; i++)
            raw[i] = rec->raw[i];
    }

    ~RecordRaw(){
        delete [] raw;
    }

};