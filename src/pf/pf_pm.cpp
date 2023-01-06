#include "pf_pm.h"
#include <cassert>
#include <unistd.h>

PfPageManager::PfPageManager() {
    for (size_t i = 0; i < MAX_CHCHE_PAGES; i++) {
        _pages[i].buf = _cache + i * PAGE_SIZE;//每页的缓冲区地址=缓冲区首地址+偏移地址
        _pages[i].is_dirty = false;
        _free_pages.push_back(&_pages[i]);//每一个空页的信息可以在_free_pages里查到
    }
}

PfPageManager::~PfPageManager() {
    flush_all();
    assert(_free_pages.size() == MAX_CHCHE_PAGES);//若假则 中止并报错
}

void PfPageManager::read_page(int fd, int page_no, uint8_t *buf, int num_bytes) {
    lseek(fd, page_no * PAGE_SIZE, SEEK_SET);//将读写位置指向从文件头往后offset个位移量
    ssize_t bytes_read = read(fd, buf, num_bytes);//从指定位置读入num_bytes长度，返回值是实际读取的长度 检查是否读取完整
    if (bytes_read != num_bytes) {
        throw UnixError();
    }
}

void PfPageManager::write_page(int fd, int page_no, const uint8_t *buf, int num_bytes) {
    lseek(fd, page_no * PAGE_SIZE, SEEK_SET);
    ssize_t bytes_write = write(fd, buf, num_bytes);
    if (bytes_write != num_bytes) {
        throw UnixError();
    }
}

Page *PfPageManager::create_page(int fd, int page_no) {
    Page *page = get_page<false>(fd, page_no);
    page->mark_dirty();
    return page;
}

Page *PfPageManager::fetch_page(int fd, int page_no) { return get_page<true>(fd, page_no); }

void PfPageManager::flush_file(int fd) {
    auto it_page = _busy_pages.begin();
    while (it_page != _busy_pages.end()) {
        auto prev_page = it_page;
        it_page++;
        if ((*prev_page)->id.fd == fd) {
            flush_page(*prev_page);
        }
    }
}

void PfPageManager::force_page(Page *page) {
    if (page->is_dirty) {
        write_page(page->id.fd, page->id.page_no, page->buf, PAGE_SIZE);
        page->is_dirty = false;
    }
}

template <bool EXISTS>
Page *PfPageManager::get_page(int fd, int page_no) {
    Page *page;
    PageId page_id(fd, page_no);
    auto map_it = _busy_map.find(page_id);//返回相应迭代器
    if (map_it == _busy_map.end()) {
        // Page is not in memory (i.e. on disk). Allocate new cache page for it.
        if (_free_pages.empty()) {
            // Cache is full. Need to flush a page to disk.
            assert(!_busy_pages.empty());
            force_page(_busy_pages.back());
           , M 
            _busy_pages.splice(_busy_pages.begin(), _busy_pages, --_busy_pages.end());
        } else {
            // Cache is not full. Allocate from free pages.
            _busy_pages.splice(_busy_pages.begin(), _free_pages, _free_pages.begin());
        }
        _busy_map[page_id] = _busy_pages.begin();//记录：已放入缓冲池
        page = _busy_pages.front();//页指针更新为_busy_page的头部
        page->id = page_id;//将该页的信息赋值
        page->is_dirty = false;
        if (EXISTS) {
            read_page(fd, page_no, page->buf, PAGE_SIZE);//读入缓存区
        }
    } else {
        // Page is in memory
        page = *map_it->second;
        access(page);
    }
    return page;
}

void PfPageManager::access(Page *page) {
    assert(in_cache(page->id));
    _busy_pages.splice(_busy_pages.begin(), _busy_pages, _busy_map[page->id]);
}

void PfPageManager::flush_page(Page *page) {
    assert(in_cache(page->id));
    auto map_it = _busy_map.find(page->id);
    auto busy_it = map_it->second;
    force_page(page);
    _free_pages.splice(_free_pages.begin(), _busy_pages, busy_it);
    _busy_map.erase(map_it);
    assert(!in_cache(page->id));
}

void PfPageManager::flush_all() {
    for (Page *page : _busy_pages) {
        force_page(page);
    }
    _free_pages.insert(_free_pages.end(), _busy_pages.begin(), _busy_pages.end());
    _busy_pages.clear();
    _busy_map.clear();
}
