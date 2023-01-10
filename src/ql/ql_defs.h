#pragma once

#include "../defs.h"

struct RecordRaw{
    uint16_t len;
    uint8_t *raw;
    bool is_null;

    RecordRaw(uint16_t rec_len){
        len = rec_len;
        raw = new uint8_t[rec_len];
        is_null = false;
    }

    RecordRaw(RecordRaw *rec){
        len = rec->len;
        is_null = rec->is_null;
        raw = new uint8_t[len];
        for(int i = 0; i < len; i++)
            raw[i] = rec->raw[i];
    }

    ~RecordRaw(){
        delete raw;
    }

};