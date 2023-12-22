#include <gtest/gtest.h>
#include "../main.hpp"

TEST(EncryptionTest, encrypt_decrypt) {
    int ret = encrypt_decrypt_test();
    EXPECT_EQ(ret, 0);
}

TEST(BeispielTest, TestFall1) {
    EXPECT_EQ(1, 1);
}

TEST(BeispielTest, TestFall2) {
    EXPECT_TRUE(true);
}
