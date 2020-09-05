#ifndef FILESYSTEM_UTILS_H
#define FILESYSTEM_UTILS_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <climits>

namespace filesystem_utils {

std::string get_current_working_directory();
std::string change_directory(const char *path);
std::string make_temporary_directory(const char *path);
void remove_tree(const char *path);

class TemporaryDirectory {
private:
    char path_[PATH_MAX] = {0};  // set zero into path_[0], and other elements gets initialized to zero.

public:
    explicit TemporaryDirectory(const char *template_);

    TemporaryDirectory(const TemporaryDirectory &) = delete;

    TemporaryDirectory &operator=(const TemporaryDirectory &) = delete;

    ~TemporaryDirectory() noexcept;

    const char *path() const noexcept;
};

class ChangeDirectory {
private:
    const std::string cwd_;
    const std::string path_;

public:
    explicit ChangeDirectory(const char *path);

    ChangeDirectory(const ChangeDirectory &) = delete;

    ChangeDirectory &operator=(const ChangeDirectory &) = delete;

    ~ChangeDirectory() noexcept;

    const char *path() const noexcept;
};

}  // namespace filesystem_utils


#endif //FILESYSTEM_UTILS_H
