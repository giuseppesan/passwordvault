#include <gtest/gtest.h>
#include "../core/main.hpp"

const std::string encrypt_path = "../testing/encrypt";


TEST(Encryption, positive)
{

    int result = encrypt_decrypt_test("Super Secret Message");
    EXPECT_EQ(result, 0);
}

TEST(Encryption, from_file_positive)
{
    int result = decrypt_from_file_test(encrypt_path, "Secret");
    EXPECT_EQ(result, 0);
}

TEST(Encryption, all_num)
{
    std::string input = "";
    char characterToAdd = 'A';
    int result;
    //1-128
    for (int count = 129; count > 1; --count)
    {
        input += characterToAdd;
        result = encrypt_decrypt_test(input);
        EXPECT_EQ(result, 0);
    }
}

TEST(Encryption, empty)
{
    std::string input = "";
    int result = encrypt_decrypt_test(input);
    EXPECT_NE(result, 0);
}

TEST(Encryption, too_big)
{
    // 129
    int result = encrypt_decrypt_test("czgfgrchydtvagxxbbdeqvxktvrcqjgdkhkapxaheefxmqqepuchekwvkvriahkkpuifgfpwmgevaqdxzycmyvwkzmmitwerqtcbzpuqvrhjugjcdhbrjbwcyrzynpjwi");
    EXPECT_NE(result, 0);
}

TEST(Credentials, add_positive) {
    encryption enc_obj;
    int result = enc_obj.add_new_entry("tag", "user", "password"); 
    EXPECT_EQ(result, 0);
}

TEST(Credentials, get_positive) {
    encryption enc_obj;
    std::string entry = "tag";
    std::string out;
    utils::read_from_file_and_find(out, entry, credentials_path);
    enc_obj.decrypt_credentials(out);
}

TEST(Credentials, get_not_found) {
    encryption enc_obj;
    std::string entry = "nothing";
    std::string out;
    utils::read_from_file_and_find(out, entry, credentials_path);
    int result = enc_obj.decrypt_credentials(out);
    EXPECT_NE(result, 0);
    std::ofstream file_credentials_path(credentials_path, std::ios::trunc);
}

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
