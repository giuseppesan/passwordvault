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
    int sha_256(string in, string &out);

    /**
     * @brief reads hashed password from file and compares it to input
     * 
     * @param name Username
     * @param pw Password
     * 
     * @return 0 is successful
    */
    int check_password(string name, string pw);

    /**
     * @brief writes the userdata in the defined format to the file <USERNAME>:<SALT>:<HASH>
     * 
     * @param name Username
     * @param password Password
     * CRYPTO_H
     * @return 0 is successful
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
    int salt_n_hash (string in, string salt, string &final_hash);

    crypto();
    ~crypto();
};
crypto::crypto() {}

crypto::~crypto() {}

#endif //CRYPTO_H