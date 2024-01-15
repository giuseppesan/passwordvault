#include <gtest/gtest.h>
#include "../core/main.hpp"

hash hash_obj;

TEST(Hashing, register_1)
{
    std::ofstream file_passwd_path(passwd_path, std::ios::trunc);
    int result = hash_obj.register_user("user1", "user1", 1);
    EXPECT_EQ(result, 0);
    std::cout << std::endl;

    result = utils::find_entry("user1", passwd_path);
    EXPECT_EQ(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, login_good_1)
{
    int result = hash_obj.check_login("user1", "user1");
    EXPECT_EQ(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, login_bad_pw_1)
{
    int result = hash_obj.check_login("user1", "wrong_pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, register_2)
{
    std::ofstream file_passwd_path(passwd_path, std::ios::trunc);
    int result = hash_obj.register_user("user2", "user2", 2);
    EXPECT_EQ(result, 0);
    std::cout << std::endl;

    result = utils::find_entry("user2", passwd_path);
    EXPECT_EQ(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, login_good_2)
{
    int result = hash_obj.check_login("user2", "user2");
    EXPECT_EQ(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, login_bad_pw_2)
{
    int result = hash_obj.check_login("user2", "wrong_pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;
}

TEST(Hashing, login_bad_all)
{
    int result = hash_obj.check_login("no_user", "pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;
    std::ofstream file_passwd_path(passwd_path, std::ios::trunc);
}