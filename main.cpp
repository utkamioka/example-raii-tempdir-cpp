#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "src/filesystem_utils.h"


int main() {
    try {
        filesystem_utils::TemporaryDirectory temp_dir("/tmp/foo-XXXXXX");
        std::cout << "create temporary directory: " << temp_dir.path() << std::endl;

        filesystem_utils::ChangeDirectory cd(temp_dir.path());
        std::cout << "change directory: " << cd.path() << std::endl;

        auto cwd = filesystem_utils::get_current_working_directory();
        std::cout << "current directory: " << cwd << std::endl;

        std::ofstream("1.txt").close();
        mkdir("2", 0777);
        std::ofstream("2/01.txt").close();
        mkdir("2/1", 0777);
        std::ofstream("2/1/001.txt").close();
        std::ofstream("2/1/002.txt").close();
        std::ofstream("3.txt").close();

        std::cout << "---- end of block ----" << std::endl;
    } catch (std::system_error &e) {
        std::cout << "received exception: (std::system_error) " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "received exception: (std::exception) " << e.what() << std::endl;
    }

    auto cwd = filesystem_utils::get_current_working_directory();
    std::cout << "current directory: " << cwd << std::endl;

    std::cout << "---- end of program ----" << std::endl;
    return 0;
}
