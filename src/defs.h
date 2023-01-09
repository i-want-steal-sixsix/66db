#pragma once

#include <cinttypes>
#include <cstring>

std::unordered_map<std::string,int> str2int{{"int",1},{"float",2},{"string",3},{"ExprOps",4},{"Column",5}};

enum ColType{
    COL_TYPE_INT,
    COL_TYPE_FLOAT,
    COL_TYPE_CHAR
};

enum ColOption{
    FIELD_OPT_NOTNULL = 0x01,
    FIELD_OPT_PRIMKEY = 0x02,
};

class Bitmap {
public:
    static const int WIDTH = 8;
    static const unsigned int HIGHEST_BIT = 0x80u;

    static void init(uint8_t *bm, int size) { memset(bm, 0, size); }

    static void set(uint8_t *bm, int pos) { bm[get_bucket(pos)] |= get_bit(pos); }

    static void reset(uint8_t *bm, int pos) { bm[get_bucket(pos)] &= (uint8_t)~get_bit(pos); }

    static bool test(const uint8_t *bm, int pos) { return (bm[get_bucket(pos)] & get_bit(pos)) != 0; }

    static int next_bit(bool bit, const uint8_t *bm, int max_n, int curr) {
        for (int i = curr + 1; i < max_n; i++) {
            if (test(bm, i) == bit) {
                return i;
            }
        }
        return max_n;
    }

    static int first_bit(bool bit, const uint8_t *bm, int max_n) { return next_bit(bit, bm, max_n, -1); }

private:
    static int get_bucket(int pos) { return pos / WIDTH; }

    static uint8_t get_bit(int pos) { return HIGHEST_BIT >> (uint8_t)(pos % WIDTH); }
};

struct Rid {
    uint32_t page_no;
    uint32_t slot_no;

    Rid() = default;
    Rid(uint32_t page_no_, uint32_t slot_no_) : page_no(page_no_), slot_no(slot_no_) {}

    friend bool operator==(const Rid &x, const Rid &y) { return x.page_no == y.page_no && x.slot_no == y.slot_no; }
    friend bool operator!=(const Rid &x, const Rid &y) { return !(x == y); }
};