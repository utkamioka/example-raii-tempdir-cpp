#include <fstream>
#include <algorithm>
#include "gtest/gtest.h"
#include "filesystem_utils.h"

class test_filesystem_utils : public ::testing::Test {
};


TEST_F(test_filesystem_utils, change_directory) {
    std::string cwd = filesystem_utils::get_current_working_directory();
    std::string prev = filesystem_utils::change_directory("/tmp");
    ASSERT_EQ(prev, cwd);
    ASSERT_EQ("/tmp", filesystem_utils::get_current_working_directory());
}

TEST_F(test_filesystem_utils, make_temporary_directory) {
    std::string path = filesystem_utils::make_temporary_directory("/tmp/test_filesystem_utils");

    std::string expect = "/tmp/test_filesystem_utils.";
    EXPECT_TRUE(path.size() == (expect.size() + std::strlen("XXXXXX")));
    EXPECT_TRUE(std::equal(std::begin(expect), std::end(expect), std::begin(path)));

    if (!filesystem_utils::list(path.c_str()).empty()) {
        ADD_FAILURE() << path << "is not empty.";
    }

    filesystem_utils::remove_tree(path.c_str());
}

TEST_F(test_filesystem_utils, make_temporary_directory__path_too_long) {
    const char *path = "/tmp/"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"
                       ;

    // mkdtemp()でエラー
    ASSERT_THROW(filesystem_utils::make_temporary_directory(path), std::system_error);
}

TEST_F(test_filesystem_utils, list) {
    std::string path = filesystem_utils::make_temporary_directory("/tmp/test_filesystem_utils");
    std::string cwd = filesystem_utils::change_directory(path.c_str());

    std::ofstream("1.txt").close();
    mkdir("2", 0777);
    std::ofstream("2/1.txt").close();
    std::ofstream("2/2.txt").close();
    std::ofstream("2/3.txt").close();
    std::ofstream("3.txt").close();
    std::ofstream("4.txt").close();

    EXPECT_EQ((std::set<std::string>{"1.txt", "2", "3.txt", "4.txt"}),
              filesystem_utils::list("."));

    EXPECT_EQ((std::set<std::string>{"1.txt", "2.txt", "3.txt"}),
              filesystem_utils::list("2"));

    EXPECT_THROW(filesystem_utils::list("1.txt"), std::system_error);
    EXPECT_THROW(filesystem_utils::list("__not_exists__"), std::system_error);

    filesystem_utils::change_directory(cwd.c_str());
    filesystem_utils::remove_tree(path.c_str());
}

TEST_F(test_filesystem_utils, TemporaryDirectory) {
    std::string path;

    {
        filesystem_utils::TemporaryDirectory temp_dir("/tmp/foo.XXXXXX");
        path = std::string(temp_dir.path());

        ASSERT_TRUE(filesystem_utils::exists(path.c_str()));
    }

    ASSERT_FALSE(filesystem_utils::exists(path.c_str()));
}

TEST_F(test_filesystem_utils, TemporaryDirectory_invalid_argument) {
    ASSERT_THROW(filesystem_utils::TemporaryDirectory temp_dir("/tmp/foo"), std::system_error);
}
