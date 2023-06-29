#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <random>
#include "../src/utils.hpp"

using namespace std;
#define ITERATIONS 10

class crypto
{
private:
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 32 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_512(string in, string &out);

    /**
     * @brief reads hashed password from file and compares it to input
     *
     * @param name Username
     * @param pw Password
     *
     * @return 0 if successful
     */
    int check_password(string name, string pw);

    /**
     * @brief writes the userdata in the defined format to the file <USERNAME>:<SALT>:<HASH>
     *
     * @param name Username
     * @param password Password
     * CRYPTO_H
     * @return 0 if successful
     */
    int register_user(string name, string password);

    /**
     * @brief creates the password hash
     * adds salts and pepper
     *
     * @param in password
     * @param salt random salt string
     * @param final_hash the hash string
     */
    int salt_n_hash(string in, string salt, string &final_hash, size_t iterations);

    /**
     * @brief checks if username is taken
     * 
     * @param name username
     * 
     * @return 0 if successful
     */
    int check_user(string name);

    crypto();
    ~crypto();
};
crypto::crypto() {}

crypto::~crypto() {}

#define SHA_256 "1"
#define SHA_512 "2"

#endif // CRYPTO_H