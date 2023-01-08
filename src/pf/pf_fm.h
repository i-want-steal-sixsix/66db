#ifndef PF_FM_H
#define PF_FM_H

#include "../error.h"
#include "pf_pm.h"
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <process.h>
#include <unordered_map>

class PfFileManager {
  public:
    static PfPageManager pager;

    static bool is_file(const std::string &path);

    static void create_file(const std::string &path);

    static void destroy_file(const std::string &path);

    static int open_file(const std::string &path);

    static void close_file(int fd);

  private:
    static std::unordered_map<std::string, int> _path2fd;
    static std::unordered_map<int, std::string> _fd2path;
};

#endif