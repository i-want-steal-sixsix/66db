#include "rm_page_handle.h"
#include "../defs.h"
#include "../sm/sm_def.h"
#include "../sm/sm_manager.h"
#include "../pf/pf_fm.h"
#include "../pf/pf_pm.h"

std::unique_ptr<FixLenRecord> RmFixLenPageHandle::get_record(uint16_t id){
    auto record = std::make_unique<FixLenRecord>(rec_size);
    uint8_t *slot = get_slot(id);
    memcpy(record->data, slot, rec_size);
    record->size = rec_size;
    return record;
}

uint16_t RmFixLenPageHandle::insert_record(uint8_t *buf){
    uint16_t newrecid = next_free_slotid();
    uint8_t *slot = get_slot(newrecid);
    memcpy(slot, buf, rec_size);
    Bitmap::set(bitmap, newrecid);

    page->buf[3]++;
    if(page->buf[3] == 0){
        page->buf[4]++;
        if(page->buf[4] == 0){
            throw PageFullError("data page");
        }
    }

    page->mark_dirty();
    return newrecid;
}

void RmFixLenPageHandle::delete_record(uint16_t id){
    Bitmap::reset(bitmap, id);

    if(page->buf[3]>0){
        page->buf[3]--;
    }
    else{
        page->buf[3] = 0xFF;
        page->buf[4]--;
    }

    page->mark_dirty();

    // 这里还可以考虑配合 sm 清除单个数据页记录

    return;
}

void RmFixLenPageHandle::update_record(uint16_t id, uint8_t *buf){
    uint8_t *slot = get_slot(id);
    memcpy(slot, buf, rec_size);
    page->mark_dirty();
    return;
}