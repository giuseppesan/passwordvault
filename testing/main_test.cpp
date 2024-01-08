#include <gtest/gtest.h>
#include "../main.hpp"

TEST(EncryptionTest, encrypt_decrypt_positive)
{

    int result = encrypt_decrypt_test("Super Secret Message");
    EXPECT_EQ(result, 0);
}
TEST(EncryptionTest, encrypt_decrypt_file_positive)
{
    int result = decrypt_from_file_test(encrypt_path, "Secret");
    EXPECT_EQ(result, 0);
}
TEST(EncryptionTest, encrypt_decrypt_all_num)
{
    std::string input = "";
    char characterToAdd = 'A';
    int result;

    for (int count = 129; count > 1; --count)
    {
        input += characterToAdd;
        result = encrypt_decrypt_test(input);
        EXPECT_EQ(result, 0);
    }
}
TEST(EncryptionTest, encrypt_decrypt_empty)
{
    std::string input = "";
    int result = encrypt_decrypt_test(input);
    EXPECT_NE(result, 0);
}
TEST(EncryptionTest, encrypt_decrypt_too_big)
{
    // 129
    int result = encrypt_decrypt_test("czgfgrchydtvagxxbbdeqvxktvrcqjgdkhkapxaheefxmqqepuchekwvkvriahkkpuifgfpwmgevaqdxzycmyvwkzmmitwerqtcbzpuqvrhjugjcdhbrjbwcyrzynpjwi");
    EXPECT_NE(result, 0);
}

TEST(EncryptionTest, hash_test)
{
    hash c_obj;
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
