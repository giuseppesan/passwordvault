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
     * @brief reads hashed password from file and compares it to input
     * 
     * @param name Username
     * @param pw Password
     * 
     * @return 0 is successful
    */
    int check_password(string name, string pw);

    /**
     * @brief writes sting to give filename
     * 
     * @param in String which is written
     * @param file Filename / Filepath
     * 
     * @return 0 is successful
    */
    int write_to_file(string in, string file);

    /**
     * @brief Reads from file
     * 
     * @param out String witch filecontent
     * 
     * @return 0 is successful
    */
    int read_from_file(string &out);

    /**
     * @brief writes the userdata in the defined format to the file <USERNAME>:<SALT>:<HASH>
     * 
     * @param name Username
     * @param password Password
     * CRYPTO_H
     * @return 0 is successful
    */
    int register_user(string name, string password);

    crypto();
    ~crypto();
};
crypto::crypto() {}

crypto::~crypto() {}

#endif //CRYPTO_H