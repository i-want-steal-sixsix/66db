#include "pf_fm.h"

std::unordered_map<std::string, int> PfFileManager::_path2fd;
std::unordered_map<int, std::string> PfFileManager::_fd2path;
PfPageManager PfFileManager::pager;

bool PfFileManager::is_file(const std::string &path) {
    struct stat st;//定义文件属性结构体
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}//0则成功且时常规文件

void PfFileManager::create_file(const std::string &path) {
    if (is_file(path)) {
        throw FileExistsError(path);
    }//存在则不允许创建
    int fd = open(path.c_str(), O_CREAT, S_IRUSR | S_IWUSR);//路径，flag=不存在则创建，mode读写
    if (fd < 0) {//文件标识符为正 open时分配
        throw UnixError();
    }
    if (close(fd) != 0) {//通过文件标识符关闭
        throw UnixError();
    }
}

void PfFileManager::destroy_file(const std::string &path) {
    if (!is_file(path)) {
        throw FileNotFoundError(path);
    }//不存在则报错

    // If file is open, cannot destroy file
    if (_path2fd.count(path)) {
        throw FileNotClosedError(path);
    }
    // Remove file from disk 没有打开或使用的话 删除目录项和内容
    if (unlink(path.c_str()) != 0) {
        throw UnixError();
    }
}

int PfFileManager::open_file(const std::string &path) {
    if (!is_file(path)) {
        throw FileNotFoundError(path);
    }
    if (_path2fd.count(path)) {
        // File is already open
        throw FileNotClosedError(path);
    }
    // Open file and return the file descriptor
    int fd = open(path.c_str(), O_RDWR);
    if (fd < 0) {
        throw UnixError();
    }
    // Memorize the opened unix file descriptor
    _path2fd[path] = fd;
    _fd2path[fd] = path;
    return fd;
}

void PfFileManager::close_file(int fd) {
    auto pos = _fd2path.find(fd);//若存在则返回一个迭代器 ，不存在则返回迭代器末尾元素
    if (pos == _fd2path.end()) {
        throw FileNotOpenError(fd);
    }
    pager.flush_file(fd);
    const std::string &filename = pos->second;
    _path2fd.erase(filename);
    _fd2path.erase(pos);
    if (close(fd) != 0) {
        throw UnixError();
    }
}
