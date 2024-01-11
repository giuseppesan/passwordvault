#include <gtest/gtest.h>
#include "../core/main.hpp"

TEST(Hashing, hash_test)
{
    hash c_obj;
    int result = 0;

    if (utils::find_entry("user1", passwd_path) == not_found)
    {
        result = c_obj.register_user("user1", "user1", 1);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;

        result = utils::find_entry("user1", passwd_path);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;
    }

    if (utils::find_entry("user2", passwd_path) == not_found)
    {
        result = c_obj.register_user("user2", "user2", 2);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;

        result = utils::find_entry("user2", passwd_path);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;
    }

    result = c_obj.check_login("user1", "user1");
    EXPECT_EQ(result, 0);
    std::cout << std::endl;

    result = c_obj.check_login("user2", "user2");
    EXPECT_EQ(result, 0);
    std::cout << std::endl;

    result = c_obj.check_login("user1", "pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;

    result = c_obj.check_login("user2", "pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;

    result = c_obj.check_login("no_user", "pass");
    EXPECT_NE(result, 0);
    std::cout << std::endl;
    std::ofstream file_passwd_path(passwd_path, std::ios::trunc);
}