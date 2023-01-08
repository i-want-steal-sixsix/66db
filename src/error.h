#pragma once

#include <cerrno>
#include <cstring>
#include <string>

class DBError : public std::exception {
    std::string msg;
public:
    DBError(const std::string &IN_msg) : msg("Error: " + IN_msg) {}

    const char *what() const noexcept override { return msg.c_str(); }
};


class FileNotOpenError : public DBError {
  public:
    FileNotOpenError(int fd) : DBError("Invalid file descriptor: " + std::to_string(fd)) {}
};

class FileNotClosedError : public DBError {
  public:
    FileNotClosedError(const std::string &filename) : DBError("File is opened: " + filename) {}
};

class FileExistsError : public DBError {
  public:
    FileExistsError(const std::string &filename) : DBError("File already exists: " + filename) {}
};

class FileNotFoundError : public DBError {
  public:
    FileNotFoundError(const std::string &filename) : DBError("File not found: " + filename) {}
};




class DatabaseNotFoundError : public DBError {
  public:
    DatabaseNotFoundError(const std::string &db_name) : DBError("Database not found: " + db_name) {}
};

class DatabaseExistsError : public DBError {
  public:
    DatabaseExistsError(const std::string &db_name) : DBError("Database already exists: " + db_name) {}
};

class DatabaseNameLengthError : public DBError {
  public:
    DatabaseNameLengthError(const std::string &db_name) : DBError("Invalid database name length: " + db_name + "(" + std::to_string(db_name.size()) + " char)") {}
};

class DatabaseNotHomeModeError : public DBError {
  public:
    DatabaseNotHomeModeError() : DBError("Database not in home mode.") {}
};

class DatabaseNotSelectedError : public DBError {
  public:
    DatabaseNotSelectedError() : DBError("Database not selected. Please use command: <USE> <tableName>;") {}
};

class TableNotFoundError : public DBError {
  public:
    TableNotFoundError(const std::string &tab_name) : DBError("Table not found: " + tab_name) {}
};

class TableExistsError : public DBError {
  public:
    TableExistsError(const std::string &tab_name) : DBError("Table already exists: " + tab_name) {}
};

class TableNameLengthError : public DBError {
  public:
    TableNameLengthError(const std::string &tab_name) : DBError("Invalid table name length: " + tab_name + "(" + std::to_string(tab_name.size()) + " char)") {}
};

class ColumnNotFoundError : public DBError {
  public:
    ColumnNotFoundError(const std::string &col_name) : DBError("Column not found: " + col_name) {}
};

class PageFullError : public DBError {
  public:
    PageFullError(const std::string &pagetype) : DBError("Page is full: " + pagetype) {}
};

class FileFullError : public DBError {
  public:
    FileFullError(const std::string &filename) : DBError("File is full: " + filename) {}
};

class WindowsError : public DBError {
  public:
    WindowsError() : DBError(strerror(errno)) {}
};