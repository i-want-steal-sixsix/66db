#pragma once

#include "../defs.h"
#include "../pf/pf.h"
#include <string.h>

static const int DAT_HEADER_SIZE = 96;

enum DatPageType{
    DAT_FIXED_LENGTH_PAGE,
};

struct FixLenPageHdr {
    // 5 Bytes
    uint8_t page_type;
    uint16_t record_size;
    uint16_t num_records;
    // 91 Bytes
    uint8_t undefined[91];

    FixLenPageHdr(uint16_t record_size_){
        page_type = DAT_FIXED_LENGTH_PAGE;
        record_size = record_size_;
        num_records = 0;
        memset(undefined, 0, sizeof(undefined));
    }

}__attribute__((packed));

struct FixLenRecord {
    uint8_t *data;
    int size;

    FixLenRecord(const FixLenRecord &other) = delete;

    FixLenRecord &operator=(const FixLenRecord &other) = delete;

    FixLenRecord(int size_) {
        size = size_;
        data = new uint8_t[size_];
    }

    ~FixLenRecord() { delete[] data; }
};
