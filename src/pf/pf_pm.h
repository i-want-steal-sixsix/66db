#ifndef  __PF_PM_H__
#define   __PF_PM_H__

#include "../error.h"
#include "pf_defs.h"
#include <list>
#include <unordered_map>

class PfPageManager {
  public:
    PfPageManager();
    ~PfPageManager();

    PfPageManager &operator=(const PfPageManager &other) = delete;

    static void read_page(int fd, int page_no, uint8_t *buf, int num_bytes);
    static void write_page(int fd, int page_no, const uint8_t *buf, int num_bytes);
    static void set_page(Page *page,uint8_t *buf,int s ,int e);
    static void blank_page(int fd,int page_no);

    Page *create_page(int fd, int page_no);

    Page *fetch_page(int fd, int page_no);

    void flush_file(int fd);
    void flush_page(Page *page);
    void flush_all();

    bool in_cache(const PageId &page_id) const { return _busy_map.find(page_id) != _busy_map.end(); }

    const std::list<Page *> &busy_list() const { return _busy_pages; }
    const std::unordered_map<PageId, std::list<Page *>::iterator> &busy_map() const { return _busy_map; }
    const std::list<Page *> &free_list() const { return _free_pages; }

  private:
    static void force_page(Page *page);

    // Get the page from memory corresponding to the disk page.
    // If the page is not in memory, allocate a page and read the disk.
    template <bool EXISTS>
    Page *get_page(int fd, int page_no);

    void access(Page *page);

  private:
    uint8_t _cache[MAX_CHCHE_PAGES * PAGE_SIZE];                                    //创建缓冲区
    Page _pages[MAX_CHCHE_PAGES];                                                   //创建页数大小的页数组 用于存放每一页的信息 isdirty和*buf
    std::unordered_map<PageId, std::list<Page *>::iterator> _busy_map;              //pageid到页指针链表的map
    std::list<Page *> _busy_pages;                                                  //页指针链表 busy
    std::list<Page *> _free_pages;
};

#endif