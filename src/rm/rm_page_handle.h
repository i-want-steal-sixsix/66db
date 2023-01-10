#pragma once

#include "../defs.h"
#include "rm_defs.h"
#include <memory>

struct RmFixLenPageHandle {
    uint16_t rec_size;
    uint16_t bmp_size;
    uint8_t *bitmap;
    uint8_t *slots;
    Page *page;

    RmFixLenPageHandle(Page *page_, uint16_t bmp_size_, uint16_t rec_size_) : page(page_), rec_size(rec_size_) {
        bmp_size = bmp_size_;
        bitmap = page->buf + DAT_HEADER_SIZE;
        slots = bitmap + bmp_size;
    }

    bool is_full() {
        return Bitmap::next_bit(0, bitmap, bmp_size, -1) == bmp_size;
    }

    bool is_empty() {
        return Bitmap::next_bit(1, bitmap, bmp_size, -1) == bmp_size;
    }

    bool is_record(uint16_t id) {
        return Bitmap::test(bitmap, id);
    }

    std::unique_ptr<FixLenRecord> get_record(uint16_t id);

    uint16_t insert_record(uint8_t *buf);

    void delete_record(uint16_t id);

    void update_record(uint16_t id, uint8_t *buf);

private:
    
    uint8_t next_free_slotid() {
        return Bitmap::next_bit(0, bitmap, bmp_size, -1);
    }

    uint8_t *get_slot(int slot_no) const { return slots + slot_no * rec_size; }


};