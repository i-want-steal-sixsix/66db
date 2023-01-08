#include "rm_manager.h"
#include "../defs.h"
#include "../sm/sm_def.h"
#include "../sm/sm_manager.h"

void RmManager::init_fixlen_page(int fd, uint16_t page_no, uint16_t record_size){
    FixLenPageHdr new_page_hdr(record_size);
    PfPageManager::write_page(fd, page_no, (uint8_t*)&new_page_hdr, DAT_HEADER_SIZE);
    return;
}
