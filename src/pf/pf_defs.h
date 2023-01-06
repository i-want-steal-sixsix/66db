#ifndef PF_DEFS_H
#define PF_DEFS_H

#include "defs.h"
#include <cinttypes>
#include <cstdlib>

//每页4k,缓冲池一次存放2^16页 共计使用256M内存
#define PAGE_SIZE 4096
#define MAX_CHCHE_PAGES  65536


class PageId{
  public:
    int fd;//文件标识符 
    int page_no;//页码

    PageId() = default;
    PageId(int _fd, int _page_no)  {
      fd = _fd;
      page_no = _page_no;
    }
    ~PageId() = default;

    //重载=运算符 使其可以比较结构体
    friend bool operator==(const PageId &x, const PageId &y) { return x.fd == y.fd && x.page_no == y.page_no; }
    friend bool operator!=(const PageId &x, const PageId &y) { return !(x == y); }

    //重载输出运算符
    friend std::ostream &operator<<(std::ostream &os, const PageId &self) {
        return os << "PageId(fd=" << self.fd << ", page_no=" << self.page_no << ")";
    }
};


class Page{
  public:
    PageId id;
    uint8_t *buf; //缓冲区的地址指针 8位
    bool is_dirty;

    void mark_dirty() { is_dirty = true; }
};

// namespace std
namespace std {
template <>
struct hash<PageId> {   //重载pageid类型的hash
    size_t operator()(const PageId &pid) const noexcept { return (pid.fd << 16) | pid.page_no; }
};
} 

#endif