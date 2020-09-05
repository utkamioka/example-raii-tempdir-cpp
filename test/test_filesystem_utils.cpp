#include <algorithm>
#include "gtest/gtest.h"
#include "filesystem_utils.h"

class test_filesystem_utils : public ::testing::Test {
};


TEST_F(test_filesystem_utils, DISABLED_fail1) {
    FAIL() << "forced fail.";
}

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

    // TODO pathが存在し、なおかつ、空のディレクトリであることを確認

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
