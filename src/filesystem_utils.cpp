#include <string>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <dirent.h>
#include "filesystem_utils.h"

namespace filesystem_utils {

namespace /* private variables */ {

using type_func_free = void(void *);
using ptr_func_free = type_func_free *;

#ifndef DEBUG
ptr_func_free str_free = std::free;
#else
void debug_str_free(void *ptr) {
    std::cerr << "free(" << (void *) ptr << ") - " << (char *) ptr << std::endl;
    std::free(ptr);
}
ptr_func_free str_free = debug_str_free;
#endif //DEBUG

}  // namespace *private variables*


namespace /* private functions */ {

// Do not throw exception, because its use in building exceptions.
std::string get_real_path(const char *path) noexcept {
    // *NOT GOOD* to specify buffer to 2nd argument of realpath(), as some environments PATH_MAX is not constant.
    auto p = std::unique_ptr<char, ptr_func_free>(realpath(path, nullptr), std::free);
    return std::string(p ? p.get() : path);
}

}  // namespace *private functions*


std::string get_current_working_directory() {
    auto cwd = std::unique_ptr<char, ptr_func_free>(get_current_dir_name(), str_free);
    if (!cwd) {
        throw std::system_error(errno, std::generic_category());
    }
    return std::string(cwd.get());
}


void remove_tree(const char *path) {
#ifdef DEBUG
    {
        char buffer[PATH_MAX];
        std::cerr << "remove " << path << " at " << getcwd(buffer, PATH_MAX) << std::endl;
    }
#endif //DEBUG

    auto is_current_dir = [](const char *path) { return strcmp(path, ".") == 0; };
    auto is_parent_dir = [](const char *path) { return strcmp(path, "..") == 0; };

    auto change_directory = [](const char *path) {
        char buffer[PATH_MAX];
        if (getcwd(buffer, PATH_MAX) == nullptr) {
            throw std::system_error(errno, std::generic_category());
        }
        if (chdir(path) != 0) {
            throw std::system_error(errno, std::generic_category(), get_real_path(path));
        }
        return std::string(buffer);
    };

    DIR *dir = nullptr;
    if ((dir = opendir(path)) != nullptr) {
        auto current_dir = change_directory(path);

        struct dirent *entry = nullptr;
        while ((entry = readdir(dir)) != nullptr) {
            if (!is_current_dir(entry->d_name) and !is_parent_dir(entry->d_name)) {
                remove_tree(entry->d_name);
            }
        }
        closedir(dir);

        change_directory(current_dir.c_str());
    } else {
        if (errno == ENOTDIR) {
            // not a directory, not an error. just simply remove it.
        } else {
            throw std::system_error(errno, std::generic_category(), get_real_path(path));
        }
    }
    if (remove(path) != 0) {
        throw std::system_error(errno, std::generic_category(), get_real_path(path));
    }
}


TemporaryDirectory::TemporaryDirectory(const char *template_) {
    strncpy(path_, template_, sizeof(path_) - 1);
    if (mkdtemp(path_) == nullptr) {
        throw std::system_error(errno, std::generic_category(), template_);
    }
}

TemporaryDirectory::~TemporaryDirectory() noexcept {
    try {
        remove_tree(path_);
    } catch (std::exception &e) {
        std::cerr << __FUNCTION__ << "(): " << e.what() << std::endl;
    } catch (...) {
        // Ignore all exceptions.
        // Because avoid terminate() when raise exception in destructor.
    }
}

const char *TemporaryDirectory::path() const noexcept {
    return path_;
}

ChangeDirectory::ChangeDirectory(const char *path) : cwd_(get_current_working_directory()), path_{path} {
    if (chdir(path) != 0) {
        throw std::system_error(errno, std::generic_category(), get_real_path(path));
    }
}

ChangeDirectory::~ChangeDirectory() noexcept {
    auto ignore_failure = chdir(cwd_.c_str());
    (void)ignore_failure;  // suppress warning of "warn_unused_result"
}

const char *ChangeDirectory::path() const noexcept {
    return path_.c_str();
}

}  // namespace filesystem_utils
