#include <gtest/gtest.h>
#include "../main.hpp"

TEST(EncryptionTest, encrypt_decrypt) {
    int result = 0;
    result = encrypt_decrypt_test();
    EXPECT_EQ(result, 0);

    result = decrypt_from_file_test("../secure/encrypt");
    EXPECT_EQ(result, 0);
}

TEST(EncryptionTest, hash_test) {
    crypto c_obj;
    int result = 0;
   
   if (c_obj.find_user("user1", 1) == 0)
    {
        result = c_obj.register_user("user1", "user1", 1);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;

        result = c_obj.find_user("user1", 1);
        EXPECT_NE(result, 0);
        std::cout << std::endl;
    }

    if (c_obj.find_user("user2", 1) == 0)
    {
        result = c_obj.register_user("user2", "user2", 2);
        EXPECT_EQ(result, 0);
        std::cout << std::endl;

        result = c_obj.find_user("user2", 1);
        EXPECT_NE(result, 0);
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

    std::ofstream file("../secure/passwd", std::ios::trunc);
}


TEST(BeispielTest, TestFall1) {
    EXPECT_EQ(1, 1);
}

TEST(BeispielTest, TestFall2) {
    EXPECT_TRUE(true);
}
