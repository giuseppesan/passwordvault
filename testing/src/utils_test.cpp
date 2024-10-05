#include <gtest/gtest.h>
#include "main.hpp"

TEST(Utils, read_from_file_and_find_bad) {
    std::string entry = "test";
    std::string out;
    int result = utils::read_from_file_and_find(out, entry, credentials_path);
    EXPECT_NE(result, 0);
}

TEST(Utils, write_to_file_and_find_good) {
    std::string entry = "test";
    std::string out;
    utils::write_to_file(entry, credentials_path);
    int result = utils::read_from_file_and_find(out, entry, credentials_path);
    EXPECT_EQ(result, 0);
    std::ofstream file_credentials_path(credentials_path, std::ios::trunc);
}

TEST(Utils, find_entry_good) {
    std::string entry = "test";
    std::string out;
    utils::write_to_file(entry, credentials_path);
    int result = utils::find_entry(entry, credentials_path);
    EXPECT_EQ(result, 0);
    std::ofstream file_credentials_path(credentials_path, std::ios::trunc);
}

