#include <gtest/gtest.h>
#include "../core/main.hpp"

TEST(Utils, not_found) {
    std::string entry = "test";
    std::string out;
    int result = utils::read_from_file_and_find(out, entry, credentials_path);
    EXPECT_NE(result, 0);
}

TEST(Utils, found) {
    std::string entry = "test";
    std::string out;
    utils::write_to_file(entry, credentials_path);
    int result = utils::read_from_file_and_find(out, entry, credentials_path);
    EXPECT_EQ(result, 0);
    std::ofstream file_credentials_path(credentials_path, std::ios::trunc);
}

TEST(Utils, found_2) {
    std::string entry = "test";
    std::string out;
    utils::write_to_file(entry, credentials_path);
    int result = utils::find_entry(entry, credentials_path);
    EXPECT_EQ(result, 0);
    std::ofstream file_credentials_path(credentials_path, std::ios::trunc);
}

