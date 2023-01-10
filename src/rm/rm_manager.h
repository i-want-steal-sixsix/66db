#pragma once

#include "rm_defs.h"
#include "rm_page_handle.h"

class RmManager {
public:
    static void init_fixlen_page(int fd, uint16_t page_no, uint16_t record_size);
};
