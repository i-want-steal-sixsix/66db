#include "rm_page_handle.h"

class FixLenScan {
public:
    FixLenScan(RmFixLenPageHandle *fh_) { now_id = -1, now_ph = fh_;}

    bool next() {
        now_id++;
        while(now_id < now_ph->bmp_size && !now_ph->is_record(now_id)){now_id++;}
        return is_end();
    }

    bool is_end() { return now_id == now_ph->bmp_size; }

    RmFixLenPageHandle *now_ph;

    uint16_t now_id;
};
