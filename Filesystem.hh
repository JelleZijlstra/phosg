#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


std::unordered_set<std::string> list_directory(const std::string& dirname,
    std::function<bool(struct dirent*)> filter = NULL);

std::string get_user_home_directory();

class cannot_stat_file : virtual public std::runtime_error {
public:
  cannot_stat_file(int fd);
  cannot_stat_file(const std::string& filename);
};

class cannot_open_file : virtual public std::runtime_error {
public:
  cannot_open_file(const std::string& filename);
};

class io_error : virtual public std::runtime_error {
public:
  io_error(int fd);
};

struct stat stat(const std::string& filename);
struct stat lstat(const std::string& filename);
struct stat fstat(int fd);
struct stat fstat(FILE* f);

bool isfile(const struct stat& st);
bool isdir(const struct stat& st);
bool lisfile(const struct stat& st);
bool lisdir(const struct stat& st);
bool islink(const struct stat& st);

bool isfile(const std::string& filename);
bool isdir(const std::string& filename);
bool lisfile(const std::string& filename);
bool lisdir(const std::string& filename);
bool islink(const std::string& filename);

std::string readlink(const std::string& filename);

void readx(int fd, void* data, size_t size);
void writex(int fd, const void* data, size_t size);
std::string readx(int fd, size_t size);
void writex(int fd, const std::string& data);

void freadx(FILE* f, void* data, size_t size);
void fwritex(FILE* f, const void* data, size_t size);
std::string freadx(FILE* f, size_t size);
void fwritex(FILE* f, const std::string& data);

std::string load_file(const std::string& filename);
void save_file(const std::string& filename, const std::string& data);

std::unique_ptr<FILE, void(*)(FILE*)> fopen_unique(const std::string& filename,
    const std::string& mode = "rb");

void unlink(const std::string& filename, bool recursive = true);

std::pair<int, int> pipe();

void make_fd_nonblocking(int fd);

class Poll {
public:
  Poll() = default;
  ~Poll() = default;

  void add(int fd, short events);
  void remove(int fd, bool close_fd = false);

  std::unordered_map<int, short> poll(int timeout_ms = 0);

private:
  std::vector<struct pollfd> poll_fds;
};
