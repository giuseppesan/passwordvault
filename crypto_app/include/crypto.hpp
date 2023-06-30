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
public:
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 32 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_256(string in, string &out);
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 64 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_512(string in, string &out);

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
     * @brief writes string to passwd file
     *
     * @param out hexstring
     *
     * @return 0 if successful
     */
    int save_password(string out);

public:
    crypto();
    ~crypto();

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
     * @brief checks if username is taken
     *
     * @param name username
     *
     * @return 0 if successful
     */
    int check_user(string name);
};

#define SHA_256 "1"
#define SHA_512 "2"

#endif // CRYPTO_H